# 🔧 Display Text on a Screen with a Different Font

**Project:** *Custom Font Digital Clock*

## 🎯 Goal

Learn how to create, load, and display **custom fonts** on a small LCD screen to make your text look unique and visually appealing.

---

## 📜 Description

This project demonstrates how to:

* Use a **parallel 16×2 character LCD** (HD44780-compatible) with **custom-designed characters**.
* Display a **digital clock** on the LCD.
* **Switch between multiple font styles** at the press of a button.

Unlike graphic displays (OLED/TFT), character LCDs have fixed-size characters.
We use the LCD’s **CGRAM (Character Generator RAM)** to store **custom pixel patterns** that replace the default look of characters.

---

## 🛠 Features

* **Two custom font styles** (Blocky and Fancy).
* **Push-button font switching** in real time.
* **Simulated clock** display (replaceable with an RTC module).

---

## 📚 What You’ll Learn

* Creating custom LCD characters using **CGRAM**.
* Dynamically switching between **multiple font sets**.
* Using a **button with debounce** logic in Arduino.

---

## 🔌 Wiring

| LCD Pin  | Arduino Pin              |
| -------- | ------------------------ |
| RS       | 12                       |
| E        | 11                       |
| D4       | 5                        |
| D5       | 4                        |
| D6       | 3                        |
| D7       | 2                        |
| RW       | GND                      |
| VSS      | GND                      |
| VDD      | 5V                       |
| VO       | Potentiometer (contrast) |
| A (LED+) | 5V via 220Ω              |
| K (LED-) | GND                      |
| Button   | Pin 8 → GND              |

---

## 💻 Code

```cpp
#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define BUTTON_PIN 8

// Custom font set 1: Blocky digits
byte zeroChar[8] = {0b01110, 0b10001, 0b10011, 0b10101, 0b11001, 0b10001, 0b01110};
byte oneChar[8]  = {0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110};

// Custom font set 2: Fancy digits
byte zeroChar2[8] = {0b01110, 0b10001, 0b10101, 0b10101, 0b10001, 0b10001, 0b01110};
byte oneChar2[8]  = {0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100};

int fontSet = 0;
unsigned long lastDebounce = 0;

void loadFontSet1() {
  lcd.createChar(0, zeroChar);
  lcd.createChar(1, oneChar);
}

void loadFontSet2() {
  lcd.createChar(0, zeroChar2);
  lcd.createChar(1, oneChar2);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  lcd.begin(16, 2);
  loadFontSet1();
}

void loop() {
  // Button handling to switch fonts
  if (digitalRead(BUTTON_PIN) == LOW && millis() - lastDebounce > 300) {
    fontSet = !fontSet;
    if (fontSet == 0) loadFontSet1();
    else loadFontSet2();
    lastDebounce = millis();
  }

  // Simulated time
  static unsigned long lastUpdate = 0;
  static int sec = 0, min = 0, hr = 12;
  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    sec++;
    if (sec >= 60) { sec = 0; min++; }
    if (min >= 60) { min = 0; hr = (hr + 1) % 24; }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Clock:");

  // Print using custom numbers for 0 and 1
  int tens = hr / 10;
  int ones = hr % 10;
  if (tens == 0) lcd.write(byte(0));
  else if (tens == 1) lcd.write(byte(1));
  else lcd.print(tens);

  if (ones == 0) lcd.write(byte(0));
  else if (ones == 1) lcd.write(byte(1));
  else lcd.print(ones);

  lcd.print(":");

  tens = min / 10;
  ones = min % 10;
  if (tens == 0) lcd.write(byte(0));
  else if (tens == 1) lcd.write(byte(1));
  else lcd.print(tens);

  if (ones == 0) lcd.write(byte(0));
  else if (ones == 1) lcd.write(byte(1));
  else lcd.print(ones);

  delay(200);
}
```

---