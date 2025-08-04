# 📚 Circular Buffer Project

## Project:

**Serial Data Logger with History Navigation**

---

## 🎯 Goal

Learn how to efficiently store and retrieve streaming sensor data using a **circular buffer** on a microcontroller.

---

## 📝 Description

* Read data from a sensor (e.g., soil moisture, temperature, light) at a fixed interval.
* Store the last ***N*** sensor readings in a fixed-size circular buffer.
* Allow the user to navigate **backward** and **forward** through stored data via serial commands:

  * `n` — show **next** reading
  * `p` — show **previous** reading

---

## 🔧 How It Works

* New sensor readings overwrite the oldest ones once the buffer is full, avoiding costly data shifting.
* `writeIndex` tracks where the next reading is stored, wrapping around the buffer size.
* `count` tracks how many readings are stored (up to buffer size).
* `currentIndex` is used to navigate through saved data on serial command input.

---

## ⚙️ Code

```cpp
// Configuration
const int SENSOR_PIN = A0;                  // Analog input pin for sensor
const unsigned long READ_INTERVAL = 2000;  // Interval between sensor reads (milliseconds)
const int BUFFER_SIZE = 10;                 // Number of readings to store in circular buffer

// Circular buffer variables
int readings[BUFFER_SIZE];  // Buffer to hold sensor readings
int writeIndex = 0;         // Index to write next reading
int count = 0;              // Number of readings stored (max BUFFER_SIZE)

// Navigation variables
int currentIndex = -1;      // Index for navigating buffer (-1 means no data yet)

unsigned long lastReadTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Soil Sensor Logger with Circular Buffer");
  Serial.println("Commands: 'n' = next, 'p' = previous");
}

void loop() {
  unsigned long now = millis();

  // Read sensor at fixed intervals
  if (now - lastReadTime >= READ_INTERVAL) {
    lastReadTime = now;

    int sensorValue = analogRead(SENSOR_PIN);
    readings[writeIndex] = sensorValue;

    writeIndex = (writeIndex + 1) % BUFFER_SIZE;

    if (count < BUFFER_SIZE) count++;

    // Update currentIndex to the newest reading
    currentIndex = (writeIndex + BUFFER_SIZE - 1) % BUFFER_SIZE;

    Serial.print("New reading stored at index ");
    Serial.print(currentIndex);
    Serial.print(": ");
    Serial.println(sensorValue);
  }

  // Serial input handling for navigation
  if (Serial.available()) {
    char cmd = Serial.read();

    if (count == 0) {
      Serial.println("No data yet.");
      return;
    }

    if (cmd == 'n') {
      // Move forward through buffer
      currentIndex = (currentIndex + 1) % count;
      printReading(currentIndex);
    } else if (cmd == 'p') {
      // Move backward through buffer
      currentIndex = (currentIndex - 1 + count) % count;
      printReading(currentIndex);
    } else {
      Serial.println("Unknown command. Use 'n' or 'p'.");
    }
  }
}

// Helper function to print reading at given index
void printReading(int index) {
  Serial.print("Reading ");
  Serial.print(index);
  Serial.print(": ");
  Serial.println(readings[index]);
}
```

---

## 📖 Usage

* Open the Serial Monitor at 9600 baud.
* The system automatically takes a sensor reading every 2 seconds.
* After some readings are stored, enter:

  * `n` + ENTER to see the **next** reading.
  * `p` + ENTER to see the **previous** reading.

---

## 🧠 Learnings

* Implementing a **fixed-size circular buffer** to efficiently handle streaming data.
* Using modulo arithmetic to **wrap indices** in the buffer.
* Handling **serial input commands** to navigate stored data history.

