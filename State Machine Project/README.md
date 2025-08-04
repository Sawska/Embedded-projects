Here’s an **enhanced version** of your README that’s cleaner, easier to read, and a bit more professional while keeping it practical:

---

# 🔦 ESP32 LED Traffic Light Controller with Pedestrian Button

## 📌 Project Overview

This project demonstrates how to design and implement a **state machine** for an **event-driven LED traffic light controller** using an **ESP32**.
It simulates a real-world traffic light system and includes a **pedestrian button** that changes the sequence to allow pedestrian crossing after the current cycle.

---

## 🎯 Goal

* Learn how to design a **clear state machine** for event-driven control.
* Practice **separating hardware code** from logic.
* Use **enums** for well-defined states.

---

## 🛠 Features

* **4 Traffic Light States:**

  * 🚦 `RED` – Stop
  * 🚦 `RED_YELLOW` – Prepare to go
  * 🚦 `GREEN` – Go
  * 🚦 `YELLOW` – Prepare to stop
* **Pedestrian button:**

  * When pressed, it will **complete the current light cycle** and then turn **RED** to allow pedestrians to cross.
* **Non-blocking timing** using `millis()` (no `delay()` used).
* **Readable, modular code** using `setState()` and `stateName()` helper functions.

---

## 📡 Hardware Connections

| LED Color | ESP32 Pin       |
| --------- | --------------- |
| 🔴 Red    | 15              |
| 🟡 Yellow | 4               |
| 🟢 Green  | 2               |
| 👣 Button | 18 (active LOW) |

---

## 📋 Code

```cpp
#define RED_PIN     15
#define YELLOW_PIN  4
#define GREEN_PIN   2
#define BUTTON_PIN  18

enum TrafficState { RED, RED_YELLOW, GREEN, YELLOW };
TrafficState currentState = RED;

bool pedestrianRequest = false; 
unsigned long stateStartTime = 0; 

// State durations (ms)
const unsigned long RED_TIME        = 4000;
const unsigned long RED_YELLOW_TIME = 1500;
const unsigned long GREEN_TIME      = 4000;
const unsigned long YELLOW_TIME     = 2000;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  setState(RED);
}

void loop() {
  // Detect pedestrian button press
  if (digitalRead(BUTTON_PIN) == LOW) {
    pedestrianRequest = true;
  }

  switch (currentState) {
    case RED:
      if (millis() - stateStartTime >= RED_TIME) setState(RED_YELLOW);
      break;
    case RED_YELLOW:
      if (millis() - stateStartTime >= RED_YELLOW_TIME) setState(GREEN);
      break;
    case GREEN:
      if (millis() - stateStartTime >= GREEN_TIME) setState(YELLOW);
      break;
    case YELLOW:
      if (millis() - stateStartTime >= YELLOW_TIME) {
        if (pedestrianRequest) pedestrianRequest = false;
        setState(RED);
      }
      break;
  }
}

void setState(TrafficState newState) {
  currentState = newState;
  stateStartTime = millis();
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  switch (newState) {
    case RED:       digitalWrite(RED_PIN, HIGH); break;
    case RED_YELLOW: digitalWrite(RED_PIN, HIGH); digitalWrite(YELLOW_PIN, HIGH); break;
    case GREEN:     digitalWrite(GREEN_PIN, HIGH); break;
    case YELLOW:    digitalWrite(YELLOW_PIN, HIGH); break;
  }

  Serial.print("State changed to: ");
  Serial.println(stateName(newState));
}

String stateName(TrafficState state) {
  switch (state) {
    case RED: return "RED";
    case RED_YELLOW: return "RED_YELLOW";
    case GREEN: return "GREEN";
    case YELLOW: return "YELLOW";
  }
  return "UNKNOWN";
}
```

---

## 📷 Demo Photo

<img width="1280" height="960" alt="Traffic Light on ESP32" src="https://github.com/user-attachments/assets/fd09fe13-6f24-4fd6-9bd9-5caf002facc1" />

