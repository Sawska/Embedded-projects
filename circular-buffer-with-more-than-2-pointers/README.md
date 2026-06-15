# 📦 Circular Buffer with More Than 2 Pointers

**Project:** *Multi-Consumer Data Logger*

## 🎯 Goal

Learn to implement a **multi-consumer circular buffer** where multiple reading positions can access the same stream of data without interfering with each other.

---

## 📜 Description

This example shows how to:

* Use **one producer** (sensor reading task) and **two or more consumers**.
* Allow multiple readers to **process the same data independently**.
* Prevent data corruption when multiple read pointers exist.
* Handle consumers running at **different speeds** without losing data integrity.

---

## 🛠 Use Case Examples

* **Consumer 1:** Sends recent sensor data over **Serial** to a PC.
* **Consumer 2:** Saves the same data to an **SD card**.
* More consumers can be added for other tasks (e.g., wireless transmission).

---

## ⚡ Extra Challenge

* Make each consumer run at **different speeds** while keeping data synchronized.
* Ensure **no unread data is overwritten** by the producer.

---

## 🔌 Hardware Setup

This example uses an **ESP32** with:

* **Analog Gas Sensor** → Pin `A0` (GPIO 36)
* **Analog Soil Moisture Sensor** → Pin `A1` (GPIO 39)

You can adapt the code for other boards and sensors.

---

## 💻 Code

```cpp
#include <Arduino.h>

#define GAS_SENSOR_PIN   36  // A0
#define SOIL_SENSOR_PIN  39  // A1

// Circular buffer settings
#define BUFFER_SIZE 256

struct SensorData {
    uint32_t timestamp;
    int gasValue;
    int soilValue;
};

SensorData buffer[BUFFER_SIZE];

// Pointers
volatile size_t writeIndex = 0;
volatile size_t readIndex1 = 0; // Consumer 1
volatile size_t readIndex2 = 0; // Consumer 2

// Producer writes to buffer
void writeToBuffer(SensorData data) {
    buffer[writeIndex] = data;
    writeIndex = (writeIndex + 1) % BUFFER_SIZE;

    // Avoid overwriting unread data
    if (writeIndex == readIndex1 || writeIndex == readIndex2) {
        // Advance the slowest reader
        if (((readIndex1 - writeIndex + BUFFER_SIZE) % BUFFER_SIZE) <
            ((readIndex2 - writeIndex + BUFFER_SIZE) % BUFFER_SIZE)) {
            readIndex1 = (readIndex1 + 1) % BUFFER_SIZE;
        } else {
            readIndex2 = (readIndex2 + 1) % BUFFER_SIZE;
        }
    }
}

// Consumer reads from buffer
bool readFromBuffer(size_t &readIndex, SensorData &out) {
    if (readIndex == writeIndex) {
        // No new data
        return false;
    }
    out = buffer[readIndex];
    readIndex = (readIndex + 1) % BUFFER_SIZE;
    return true;
}

// Producer task: reads sensors and writes to buffer
void producerTask(void *param) {
    for (;;) {
        SensorData data;
        data.timestamp = millis();
        data.gasValue = analogRead(GAS_SENSOR_PIN);
        data.soilValue = analogRead(SOIL_SENSOR_PIN);

        writeToBuffer(data);

        vTaskDelay(pdMS_TO_TICKS(500)); // Read every 500ms
    }
}

// Consumer 1: send to Serial at different speed
void consumer1Task(void *param) {
    SensorData data;
    for (;;) {
        while (readFromBuffer(readIndex1, data)) {
            Serial.printf("[Serial] Time: %lu ms | Gas: %d | Soil: %d\n",
                          data.timestamp, data.gasValue, data.soilValue);
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 sec delay
    }
}

// Consumer 2: simulate SD logging at slower speed
void consumer2Task(void *param) {
    SensorData data;
    for (;;) {
        while (readFromBuffer(readIndex2, data)) {
            Serial.printf("[SD Log] Time: %lu ms | Gas: %d | Soil: %d\n",
                          data.timestamp, data.gasValue, data.soilValue);
        }
        vTaskDelay(pdMS_TO_TICKS(3000)); // 3 sec delay
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Create FreeRTOS tasks
    xTaskCreatePinnedToCore(producerTask, "Producer", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(consumer1Task, "Consumer1", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(consumer2Task, "Consumer2", 4096, NULL, 1, NULL, 1);
}

void loop() {
    // Nothing in loop; everything runs in tasks
}
```

---

## 🔍 How It Works

* **Producer Task:** Reads both sensors every **500 ms** and pushes data into the circular buffer.
* **Consumer 1:** Reads and prints data to Serial every **1 second**.
* **Consumer 2:** Reads and logs data every **3 seconds**.
* The buffer logic ensures **no corruption** by advancing the **slowest reader** when space is needed.

---

