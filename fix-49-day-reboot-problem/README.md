# 🕒 Fix 49-Day Reboot Problem  
**Project: Long Uptime Weather Logger**

---

## 🧾 Overview

This project demonstrates how to create a **robust weather logger** that can run indefinitely on an **ESP32 + DS3231 RTC** without crashing due to `millis()` overflow (~49.7 days). It logs the temperature **every minute** using the DS3231's onboard sensor and timestamps each reading using **calendar time**, not millisecond counters.

---

## 🧠 Skills Practiced

- Avoiding `millis()`/`delay()` overflow issues  
- Using Unix timestamp (`unixtime()`) for long-term scheduling  
- Reading temperature from DS3231's built-in sensor  
- Interfacing ESP32 with external I2C RTC  
- Writing non-blocking, reliable loop timing logic

---

## 🧰 Hardware Required

- ✅ ESP32 (any dev board)  
- ✅ DS3231 RTC Module  
- 🔌 Jumper wires  
- 🔋 Optional: battery for RTC backup (CR2032)

**Wiring (I2C default on ESP32):**

| RTC Pin | ESP32 Pin |
|---------|-----------|
| SDA     | GPIO21    |
| SCL     | GPIO22    |
| VCC     | 3.3V      |
| GND     | GND       |

---

## 📦 Libraries Used

Install via Arduino Library Manager:

- [`RTClib`](https://github.com/adafruit/RTClib) – for DS3231 RTC functions  
- `Wire` – for I2C communication (included by default)

---

## 📄 Code

```cpp
#include <Wire.h>
#include "RTClib.h"

// RTC instance
RTC_DS3231 rtc;

// Last time a temperature log was made
DateTime lastLogTime;

void setup() {
  Serial.begin(115200);
  Wire.begin(); // SDA: GPIO21, SCL: GPIO22

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to compile time
  }

  lastLogTime = rtc.now(); // Initialize log timestamp
}

void loop() {
  DateTime now = rtc.now();

  // Check if at least one minute has passed
  if ((now.unixtime() - lastLogTime.unixtime()) >= 60) {
    logTemperature(now);
    lastLogTime = now;
  }

  delay(500); // Minimal idle delay
}

void logTemperature(const DateTime& timestamp) {
  float tempC = rtc.getTemperature();

  Serial.print("Time: ");
  Serial.print(timestamp.timestamp());
  Serial.print(" | Temp: ");
  Serial.print(tempC);
  Serial.println(" °C");

  // Future: store to SD card, send via LoRa, etc.
}
```

---

## 💡 Why This Matters

Using `millis()` or `delay()` works for short uptime, but both **overflow after 2³² ms (~49.7 days)**, which causes bugs and unpredictable behavior in long-running systems.

✅ By using `rtc.now().unixtime()` instead:
- You avoid overflow
- You gain human-readable timestamps
- You can log or sync data accurately for months or years

---

