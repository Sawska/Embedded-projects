# 🌿 MCU Sleep in Low-Power Project

**Battery-Powered Environmental Logger**

---

## 🧾 Project Summary

A low-power data logging project using the **Arduino Uno** and a **DS18B20 temperature sensor**. The microcontroller wakes up every **10 minutes**, logs the temperature, and then returns to **sleep mode** to conserve battery power. This is an ideal starting point for battery-efficient sensor nodes in remote or IoT applications.

---

## 🧠 Skills Practiced

* Using the `LowPower.h` library for deep sleep on the ATmega328P
* Periodic wake-ups using timed sleep loops
* Interfacing with a **DS18B20** digital temperature sensor
* Basic power optimization techniques for microcontrollers

---

## 🧰 Hardware Requirements

* Arduino Uno
* DS18B20 temperature sensor
* 4.7kΩ pull-up resistor (between data and VCC)
* Battery supply (optional, for deployment)

---

## 📦 Libraries Used

Install via Arduino Library Manager:

* [`LowPower`](https://github.com/rocketscream/Low-Power) – for putting the MCU into power-down sleep mode
* `OneWire` – for 1-Wire communication protocol
* `DallasTemperature` – for communicating with the DS18B20 sensor

---

## 💻 Code

```cpp
#include <LowPower.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin connected to the DS18B20 temperature sensor
const int ONE_WIRE_BUS = 2;

// OneWire and temperature sensor setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();
}

void loop() {
  // Wake up and take temperature reading
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C");

  // Sleep for 10 minutes (600,000 ms)
  // Since LowPower.powerDown() maxes out at 8 seconds,
  // we loop it 75 times to achieve ~10 minutes
  for (int i = 0; i < 75; i++) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}
```

---

## 🔋 Power Optimization Tips

* Remove power-hungry components like status LEDs (e.g., pin 13).
* Disable unused peripherals (ADC, BOD) during sleep.
* Consider using an **external RTC module** (e.g., DS3231) with interrupt-based wakeup for better timing and lower power.
* Replace Arduino Uno with **ATmega328P standalone** or **Pro Mini (3.3V)** for even lower consumption.

---

## 📈 Applications

* Remote environmental sensing
* IoT sensor nodes
* Weather stations
* Plant monitoring systems

---

## 🛠 Future Improvements

* Add SD card or EEPROM logging
* Use an RTC (Real-Time Clock) for more accurate and efficient sleep scheduling
* Transmit data via LoRa/ESP8266/NRF24L01 modules
* Implement battery voltage monitoring

