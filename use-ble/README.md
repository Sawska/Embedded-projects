# 📡 Use BLE – Gas Sensor Beacon

**Project:** *BLE Gas Sensor Beacon*

## 🎯 Goal

Learn how to broadcast **gas sensor data** over **Bluetooth Low Energy (BLE)** using the ESP32, making it visible to smartphones or other BLE‑enabled devices.

---

## 📜 Description

This project demonstrates how to:

* Use the **ESP32’s built‑in BLE** to read an analog **MQ‑series gas sensor**.
* Advertise the gas level in a **BLE GATT service** so it can be viewed on a phone.
* Monitor the sensor in real time using apps like **nRF Connect** (iOS/Android).

While the original concept was for a *BLE Temperature Beacon*, this version replaces the temperature sensor with a **gas sensor** for air quality monitoring.

---

## 🛠 Features

* Reads gas level from **analog pin**.
* Sends updated readings every **2 seconds** via BLE notifications.
* Can be viewed in **nRF Connect** without complex pairing.

---

## 📚 What You’ll Learn

* **BLE advertising** vs **GATT services**.
* Broadcasting sensor data without pairing.
* Reading BLE data from a phone in real time.

---

## 🔌 Hardware Setup

| MQ Gas Sensor Pin | ESP32 Pin  |
| ----------------- | ---------- |
| VCC               | 5V         |
| GND               | GND        |
| AOUT              | GPIO 34    |
| DOUT              | *(unused)* |

---

## 💻 Code

```cpp
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define GAS_SENSOR_PIN 34 // Analog pin for MQ gas sensor

// BLE Service and Characteristic UUIDs (custom)
#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "12345678-1234-5678-1234-56789abcdef1"

BLEServer* pServer;
BLECharacteristic* pCharacteristic;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // BLE initialization
  BLEDevice::init("ESP32 Gas Beacon");
  pServer = BLEDevice::createServer();
  
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->start();

  Serial.println("BLE Gas Sensor Beacon started. Open nRF Connect to view data.");
}

void loop() {
  // Read gas sensor value
  int gasValue = analogRead(GAS_SENSOR_PIN);

  // Convert to string for BLE
  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%d", gasValue);

  // Send BLE notification
  pCharacteristic->setValue((uint8_t*)buffer, strlen(buffer));
  pCharacteristic->notify();

  Serial.printf("Gas Level: %d\n", gasValue);

  delay(2000); // Send every 2 seconds
}
```

---

## 📲 Viewing the Data

1. Upload the code to your ESP32.
2. Open **nRF Connect** (iOS/Android).
3. Scan for `"ESP32 Gas Beacon"`.
4. Connect → Locate the custom service → Enable **notifications**.
5. Watch gas readings update in real time.

---
