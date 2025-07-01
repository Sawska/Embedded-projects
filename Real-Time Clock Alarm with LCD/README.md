# ⏰ Real-Time Clock Alarm with LCD & Relay Control

**🧰 Board:** Raspberry Pi Pico / Arduino Uno
**📦 Modules Used:**

* DS3231 Real-Time Clock (RTC)
* 16x2 LCD Display with I2C Interface
* Relay Module (to control external devices)
* Active Buzzer
* Push Buttons (Mode, Up, Down)

---

## 💡 Skills Demonstrated

* I²C communication with RTC and LCD
* Real-time clock synchronization using DS3231
* Relay module control to switch external appliances
* Audible alarms using a buzzer
* User interface with push buttons for setting time and alarms

---

## 📖 Project Overview

This project is a real-time clock alarm system built around the **Raspberry Pi Pico** or **Arduino Uno**. It allows users to:

* View and set the current time
* Set an alarm time
* Trigger a relay and buzzer when the alarm goes off

The time and alarm can be configured through push buttons. When the current time matches the alarm time, the relay (to control devices like lights or fans) is activated and the buzzer sounds for 30 seconds or until the user stops it.

---

## 🔌 Circuit Diagram (Wokwi Simulation)

### 🖼️ Diagram:

![RTC Alarm Wokwi](https://github.com/user-attachments/assets/f0595be2-fb95-44a7-8b15-2768957fc09e)

### 🔗 [Live Wokwi Simulation](https://wokwi.com/projects/435162910463836161)

---

## 🔧 Pin Configuration (Pico)

| Component     | Pico GPIO | Function        |
| ------------- | --------- | --------------- |
| Relay IN      | GP26      | Alarm output    |
| Buzzer (+)    | GP16      | Sound output    |
| Button (MODE) | GP15      | UI navigation   |
| I2C SDA       | GP0       | LCD/RTC I2C SDA |
| I2C SCL       | GP1       | LCD/RTC I2C SCL |

> Note: `BTN_UP` and `BTN_DOWN` are not wired in the basic Wokwi version, but can be easily added for full functionality.

---

## 🧠 Example Code

<details>
<summary>Click to view Pico-compatible Arduino code</summary>

```cpp
// Shortened preview – full code is inside the project folder
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

const uint8_t RELAY_PIN = 26;  // GP26
const uint8_t BUZZER_PIN = 16; // GP16
const uint8_t BTN_MODE = 15;   // GP15

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

// ... (see full code in .ino file)
```

</details>

---

## 🚀 Getting Started

1. **Install Libraries in Arduino IDE**

   * `RTClib` by Adafruit
   * `LiquidCrystal_I2C` by John Rickman

2. **Upload Code**

   * Select your board (e.g., *Raspberry Pi Pico* or *Arduino Uno*)
   * Connect hardware as per the circuit
   * Upload the `.ino` file

3. **Test It**

   * Watch the time display on the LCD
   * Use buttons to set current time and alarm
   * Observe buzzer and relay activation at alarm time

---

## 🛠️ Future Improvements

* Add EEPROM/flash save for alarm time
* Add snooze button support
* Support for 12/24-hour display toggle
* Multi-alarm functionality
