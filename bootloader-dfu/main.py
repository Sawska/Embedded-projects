#!/usr/bin/env python3
from __future__ import annotations

import argparse
import os
import shlex
import subprocess
import sys
import time
from dataclasses import dataclass
from typing import List, Optional, Tuple

try:
    import serial  # type: ignore
    from serial.tools import list_ports  # type: ignore
except Exception as e:
    print("[!] pyserial is required. Install with: pip install pyserial", file=sys.stderr)
    raise



@dataclass
class PortInfo:
    device: str
    description: str
    vid: Optional[int]
    pid: Optional[int]


def list_serial_ports() -> List[PortInfo]:
    ports = []
    for p in list_ports.comports():
        ports.append(
            PortInfo(
                device=p.device,
                description=p.description or "",
                vid=getattr(p, "vid", None),
                pid=getattr(p, "pid", None),
            )
        )
    return ports


def print_ports():
    print("Available serial ports:\n")
    for p in list_serial_ports():
        vidpid = (
            f"VID:PID={p.vid:04X}:{p.pid:04X}" if p.vid is not None and p.pid is not None else "VID:PID=unknown"
        )
        print(f"  {p.device:>12}  | {vidpid} | {p.description}")
    if not list_serial_ports():
        print("  (none found)")


# --------------------------- Bootloader entry -------------------------

def toggle_dtr_rts(port: str, dtr: Optional[bool] = None, rts: Optional[bool] = None, delay: float = 0.05):
    """Open the port briefly and toggle DTR/RTS lines."""
    with serial.Serial(port, 115200, timeout=0.1) as ser:
        if dtr is not None:
            ser.dtr = dtr
        if rts is not None:
            ser.rts = rts
        ser.flush()
        time.sleep(delay)


def enter_bootloader_uno(port: str):
    """Arduino UNO (Optiboot) enters bootloader on reset (DTR low pulse)."""
    try:
        # Pulse DTR low then high.
        toggle_dtr_rts(port, dtr=True)
        time.sleep(0.05)
        toggle_dtr_rts(port, dtr=False)
        time.sleep(0.2)  # brief window for bootloader
    except Exception as e:
        print(f"[!] Failed to toggle DTR for UNO: {e}")


def enter_bootloader_esp32(port: str):
    """ESP32 bootloader entry via RTS/DTR (common USB-serial wiring):
    Typical mapping: RTS->EN (RESET), DTR->IO0. To enter bootloader,
    hold IO0 low (DTR True), reset low (RTS True), then release reset (RTS False),
    then release IO0 (DTR False).
    """
    try:
        # Step 1: both low
        toggle_dtr_rts(port, dtr=True, rts=True)
        time.sleep(0.05)
        # Step 2: release reset (RTS False)
        toggle_dtr_rts(port, rts=False)
        time.sleep(0.05)
        # Step 3: release IO0 (DTR False)
        toggle_dtr_rts(port, dtr=False)
        time.sleep(0.1)
    except Exception as e:
        print(f"[!] Failed to toggle DTR/RTS for ESP32: {e}")


# --------------------------- Flash commands --------------------------

@dataclass
class FlashResult:
    ok: bool
    cmd: str
    returncode: int


def run_cmd(cmd: List[str]) -> FlashResult:
    print("$", " ".join(shlex.quote(c) for c in cmd))
    try:
        p = subprocess.run(cmd, check=False)
        return FlashResult(ok=p.returncode == 0, cmd=" ".join(cmd), returncode=p.returncode)
    except FileNotFoundError:
        print(f"[!] Command not found: {cmd[0]}")
        return FlashResult(ok=False, cmd=" ".join(cmd), returncode=127)


# --- Arduino UNO (AVR) ---

def flash_uno(port: str, hex_path: str, baud: int = 115200, mcu: str = "atmega328p") -> FlashResult:
    if not os.path.exists(hex_path):
        print(f"[!] HEX not found: {hex_path}")
        return FlashResult(False, "", 1)

    enter_bootloader_uno(port)

    avrdude = os.environ.get("AVRDUDE", "avrdude")
    args = [
        avrdude,
        "-v",
        "-p", mcu,
        "-c", "arduino",
        "-P", port,
        "-b", str(baud),
        "-D",
        "-U", f"flash:w:{hex_path}:i",
    ]
    return run_cmd(args)


# --- ESP32 ---

def build_esptool_cmd(port: str, baud: int, mappings: List[Tuple[int, str]], chip: str = "esp32") -> List[str]:
    esptool = os.environ.get("ESPTOOL", "esptool.py")
    cmd = [
        esptool,
        "--chip", chip,
        "--port", port,
        "--baud", str(baud),
        "write_flash",
        "-z",
    ]
    # Flatten pairs: [addr, file, addr, file, ...]
    for addr, path in mappings:
        cmd.extend([hex(addr), path])
    return cmd


def flash_esp32(
    port: str,
    bin_path: str,
    baud: int = 921600,
    chip: str = "esp32",
    offset: int = 0x10000,
    extra_mappings: Optional[List[Tuple[int, str]]] = None,
) -> FlashResult:
    if not os.path.exists(bin_path):
        print(f"[!] BIN not found: {bin_path}")
        return FlashResult(False, "", 1)

    # Try to enter bootloader mode via DTR/RTS dance
    enter_bootloader_esp32(port)

    mappings = [(offset, bin_path)]
    if extra_mappings:
        mappings = extra_mappings + mappings

    cmd = build_esptool_cmd(port, baud, mappings, chip=chip)
    return run_cmd(cmd)


# --------------------------- CLI -------------------------------------

def auto_detect_board(port: Optional[str]) -> Optional[str]:
    """Best-effort auto-detect: returns 'uno' or 'esp32' or None."""
    ports = list_serial_ports()
    candidates = [p for p in ports if (port is None or p.device == port)]
    # Known patterns (VID:PID)
    UNO_VIDS = {0x2341, 0x2A03}  # Arduino
    UNO_BRIDGES = {0x1A86, 0x0403, 0x10C4}  # CH340, FTDI, CP210x (generic)
    ESP_BRIDGES = {0x1A86, 0x10C4, 0x0403, 0x303A}  # CH34x, CP210x, FTDI, Espressif

    for p in candidates:
        if p.vid in UNO_VIDS:
            return "uno"
    for p in candidates:
        if p.vid in ESP_BRIDGES:
            # Could be ESP or UNO clone; ambiguous. Default to ESP32 only if description hints.
            if "ESP32" in p.description.upper() or "CP210" in p.description.upper() or "ESPRESSIF" in p.description.upper():
                return "esp32"
    return None


def main():
    parser = argparse.ArgumentParser(description="Custom Firmware Loader for Arduino UNO & ESP32")
    sub = parser.add_subparsers(dest="cmd", required=True)

    p_list = sub.add_parser("list", help="List serial ports")

    p_flash = sub.add_parser("flash", help="Flash firmware")
    p_flash.add_argument("--board", choices=["uno", "esp32"], help="Target board type")
    p_flash.add_argument("--port", required=False, help="Serial port (e.g., COM7 or /dev/ttyUSB0)")
    p_flash.add_argument("--file", required=True, help="Path to firmware (HEX for UNO, BIN for ESP32)")
    p_flash.add_argument("--baud", type=int, help="Baud rate (default 115200 for UNO, 921600 for ESP32)")
    # ESP32-specific
    p_flash.add_argument("--chip", default="esp32", help="ESP chip (esp32, esp32s2, esp32c3, ...)")
    p_flash.add_argument("--esp32-offset", type=lambda x: int(x, 0), default=None, help="App offset (e.g., 0x10000)")
    p_flash.add_argument(
        "--esp32-mapping",
        action="append",
        default=[],
        help=(
            "Add an address:file mapping (ESP32). Example: --esp32-mapping 0x1000:bootloader.bin "
            "--esp32-mapping 0x8000:partitions.bin"
        ),
    )

    args = parser.parse_args()

    if args.cmd == "list":
        print_ports()
        return 0

    # cmd == flash
    port = args.port
    if port is None:
        # Choose the first available port (best-effort)
        ports = list_serial_ports()
        if not ports:
            print("[!] No serial ports found. Use --port to specify.")
            return 2
        port = ports[0].device
        print(f"[*] Using first detected port: {port}")

    board = args.board
    if board is None:
        board = auto_detect_board(port)
        if board:
            print(f"[*] Auto-detected board: {board}")
        else:
            print("[!] Could not auto-detect board. Please specify --board uno|esp32")
            return 2

    if board == "uno":
        baud = args.baud or 115200
        print(f"[*] Flashing Arduino UNO on {port} at {baud}...")
        res = flash_uno(port, args.file, baud=baud)
        if res.ok:
            print("[✓] Flash successful (UNO)")
        else:
            print(f"[x] Flash failed (UNO). Return code: {res.returncode}")
        return 0 if res.ok else 1

    if board == "esp32":
        baud = args.baud or 921600
        chip = args.chip
        # Parse mappings
        mappings: List[Tuple[int, str]] = []
        for m in args.esp32_mapping:
            try:
                addr_str, file_str = m.split(":", 1)
                addr = int(addr_str, 0)
                if not os.path.exists(file_str):
                    print(f"[!] Mapping file not found: {file_str}")
                    return 2
                mappings.append((addr, file_str))
            except ValueError:
                print(f"[!] Invalid mapping: {m}. Use format 0xADDR:path.bin")
                return 2
        # If no explicit mappings, flash single app at offset (default 0x10000)
        offset = args.esp32_offset if args.esp32_offset is not None else 0x10000

        print(f"[*] Flashing ESP32 ({chip}) on {port} at {baud}...")
        res = flash_esp32(port, args.file, baud=baud, chip=chip, offset=offset, extra_mappings=mappings)
        if res.ok:
            print("[✓] Flash successful (ESP32)")
        else:
            print(f"[x] Flash failed (ESP32). Return code: {res.returncode}")
        return 0 if res.ok else 1

    print("[!] Unknown board type.")
    return 2


if __name__ == "__main__":
    sys.exit(main())
