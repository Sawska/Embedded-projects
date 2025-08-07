# 🚀 Assembly-Optimised LED Blinker

## 📦 Project Overview

This project demonstrates how to write an **LED blink delay loop entirely in Xtensa assembly** on an **ESP32** microcontroller. It compares performance and control between a traditional C implementation and a low-level, cycle-accurate hand-optimized assembly version.

> 💡 Ideal for embedded developers interested in:
>
> * Learning Xtensa assembly
> * MCU cycle-level optimization
> * Low-level GPIO manipulation on the ESP32

---

## 🛠 Hardware

* **Board:** ESP32-WROOM-32 (or any ESP32 dev board)
* **LED Pin:** GPIO2 (changeable via `LED_MASK`)


---

## 🧠 Skills Practiced

* Writing delay loops in raw assembly
* Inline Xtensa assembly with GCC
* Register-level GPIO control
* Performance tuning on microcontrollers

---

## 🔩 Dependencies

* [ESP-IDF](https://github.com/espressif/esp-idf) (recommended)
* Or use [Arduino-ESP32](https://github.com/espressif/arduino-esp32) with appropriate inline ASM support
* Serial monitor for observing LED behavior

---

## 💡 How It Works

This blinker toggles an LED connected to GPIO2:

* Uses **`REG_WRITE()`** to directly write to ESP32 GPIO set/clear registers.
* The **delay loop** is implemented using inline Xtensa ASM (`__asm__ __volatile__`) for deterministic timing.
* No high-level function calls (like `gpio_set_level()` or `vTaskDelay()`) are used for maximum performance and predictability.

---

## 🔍 Code

```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GPIO_OUT_W1TS_REG 0x3FF44008  // Write 1 to set GPIO
#define GPIO_OUT_W1TC_REG 0x3FF4400C  // Write 1 to clear GPIO
#define LED_MASK (1 << 2)             // GPIO2 mask

void delay_loop() {
    __asm__ __volatile__ (
        "movi a2, 1000000\n"     // Load loop counter
        "1:\n"
        "addi a2, a2, -1\n"
        "bnez a2, 1b\n"          // Loop until zero
    );
}

void app_main() {
    REG_WRITE(GPIO_OUT_W1TC_REG, LED_MASK); // Ensure LED is off at start

    while (1) {
        // Turn LED on
        __asm__ __volatile__ (
            "movi a3, %[led]\n"
            "movi a2, %[set_reg]\n"
            "s32i a3, a2, 0\n"
            :
            : [led] "i" (LED_MASK), [set_reg] "r" (GPIO_OUT_W1TS_REG)
        );

        delay_loop();

        // Turn LED off
        __asm__ __volatile__ (
            "movi a3, %[led]\n"
            "movi a2, %[clr_reg]\n"
            "s32i a3, a2, 0\n"
            :
            : [led] "i" (LED_MASK), [clr_reg] "r" (GPIO_OUT_W1TC_REG)
        );

        delay_loop();
    }
}
```

---

## 🧪 How to Run

### ESP-IDF

```bash
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```





## 📜 License

This project is licensed under the **MIT License**. Use freely for educational or personal purposes.

