# 🛠 Production Line Board Tester

## 📌 Project Overview

This project provides **manufacturing test software** for an **Arduino Uno** (or compatible boards) that verifies essential board functionality during production.
It focuses on **LED** and **Button** testing for quick functional verification.

---

## 🎯 Goal

* Learn how to design **automated functional tests** for hardware.
* Provide a **pass/fail indication** for each test.
* Make it easy for operators to run the test and verify results.

---

## 🛠 Features

* **LED Test** – Blinks the LED to verify functionality.
* **Button Test** – Waits for operator input to confirm button works.
* **Clear PASS/FAIL output** in Serial Monitor.
* **Single-run design** – Tests run once per power cycle.

---

## 🔌 Hardware Requirements

| Component   | Notes                                     |
| ----------- | ----------------------------------------- |
| Arduino Uno | Or ATmega328P-based board                 |
| LED         | Can be built-in (`pin 13`) or external    |
| Push Button | Wired to the defined `BUTTON_PIN`         |
| USB Cable   | For programming and Serial Monitor output |

---

## 📡 How It Works

1. **LED Test** – The LED blinks twice. The operator visually checks it.
2. **Button Test** – The program waits **5 seconds** for the button to be pressed.
3. **Serial Output** – Displays the result of each test and an overall pass/fail.

---

## 📋 Example Output

```
=== LED & Button Manufacturing Test ===
Testing LED... Should blink twice.
LED Test: PASS
Press the button within 5 seconds...
Button Test: PASS
=========================
✅ BOARD PASSED TESTS
=========================
```

---

## 💻 Code

```cpp
// =====================
// CONFIGURATION
// =====================
#define LED_PIN 13
#define BUTTON_PIN 2

bool test_pass = true; // Track if board passes all tests

// =====================
// UTILITY FUNCTION
// =====================
void printResult(const char* testName, bool result) {
  Serial.print(testName);
  Serial.print(": ");
  if (result) {
    Serial.println("PASS");
  } else {
    Serial.println("FAIL");
    test_pass = false;
  }
}

// =====================
// LED TEST
// =====================
bool testLED() {
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Testing LED... Should blink twice.");
  
  digitalWrite(LED_PIN, HIGH); delay(300);
  digitalWrite(LED_PIN, LOW);  delay(300);
  digitalWrite(LED_PIN, HIGH); delay(300);
  digitalWrite(LED_PIN, LOW);  delay(300);

  return true; // Assumes visual confirmation
}

// =====================
// BUTTON TEST
// =====================
bool testButton() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println("Press the button within 5 seconds...");

  unsigned long start = millis();
  while (millis() - start < 5000) {
    if (digitalRead(BUTTON_PIN) == LOW) {
      return true; // Button pressed
    }
  }
  return false; // Timeout
}

// =====================
// SETUP
// =====================
void setup() {
  Serial.begin(9600);
  Serial.println("=== LED & Button Manufacturing Test ===");

  printResult("LED Test", testLED());
  printResult("Button Test", testButton());

  Serial.println("=========================");
  if (test_pass) {
    Serial.println("✅ BOARD PASSED TESTS");
  } else {
    Serial.println("❌ BOARD FAILED TESTS");
  }
  Serial.println("=========================");
}

// =====================
// LOOP
// =====================
void loop() {
  // No loop actions — single run per power cycle
}
```

---

## 🚀 Possible Improvements

* Add **automatic LED verification** using a **light sensor**.
* Save results to an **SD card** for manufacturing logs.
* Include **buzzer or status LED** for clear pass/fail feedback without a computer.
* Expand to test **I²C, SPI, UART, and analog pins** for full board validation.


