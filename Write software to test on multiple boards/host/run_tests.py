import serial, time, json, sys, re

def find_port_guess():
    # quick heuristic: change to your platform's port
    ports = ['/dev/ttyACM0','/dev/ttyUSB0','/dev/tty.usbserial','COM3']
    for p in ports:
        try:
            s = serial.Serial(p, 115200, timeout=1)
            s.close()
            return p
        except:
            pass
    raise RuntimeError("No serial port found; edit run_tests.py to add candidate ports")

def run(port, baud=115200, timeout=10):
    s = serial.Serial(port, baud, timeout=1)
    start = time.time()
    results = []
    while time.time() - start < timeout:
        line = s.readline().decode(errors='ignore').strip()
        if not line: continue
        print(">>", line)
        results.append(line)
        if line == "TEST:END":
            break
    s.close()
    # analyze results
    passed = any(l=="TEST:GPIO:PASS" for l in results) and any("TEST:I2C:PASS" in l for l in results)
    print("RESULT:", "PASS" if passed else "FAIL")
    return passed, results

if __name__=="__main__":
    port = sys.argv[1] if len(sys.argv)>1 else find_port_guess()
    run(port)
