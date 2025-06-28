# 🚨 Motion-Activated Alert System with LCD

A simple embedded systems project using **Arduino Uno** and **ESP32**, designed to trigger visual and audible alerts when motion or proximity is detected. Ideal for learning basic sensor integration, alarm systems, and GPIO control.

---

## 🔧 Features

* **Microcontrollers**: Arduino Uno & ESP32
* **Sensors & Actuators**:

  * HC-SR501 PIR Motion Sensor
  * HC-SR04 Ultrasonic Distance Sensor
  * Buzzer (for audible alert)
  * Red LED (for visual alert)
  * I2C LCD Display
* **Core Skills**:

  * GPIO Input/Output
  * Sensor integration
  * Distance measurement
  * LCD display usage
  * Alarm logic with delay
  * *(Bonus)* IR Remote toggle system ON/OFF

---

## 📋 Description

When motion is detected or an object is detected within **24 cm**, the **buzzer sounds**, the **LED lights up**, and the **distance is displayed on the LCD**. After a short delay, the system resets and continues monitoring.



---

## 🧩 ESP32 Implementation

### 📸 Real Hardware Setup



---

### 🎥 Video Demonstration


---

### 🧠 ESP32 Code with Ultrasonic Sensor and LCD

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD: address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define pin connections
const int trigPin = 5;
const int echoPin = 18;
const int buzzerPin = 2;
const int ledPin = 4;

const int distanceThreshold = 24;  // cm

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Initialize I2C for ESP32 (SDA = 21, SCL = 22)
  Wire.begin(21, 22);

  // Initialize the LCD
  lcd.begin(16, 2, LCD_5x8DOTS);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
}

void loop() {
  // Trigger the ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo time
  long duration = pulseIn(echoPin, HIGH);

  // Convert time to distance (cm)
  float distance = duration * 0.034 / 2;

  // Update LCD with distance
  lcd.setCursor(0, 1);
  lcd.print("                ");  // Clear previous data
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");

  // Trigger buzzer and LED if too close
  if (distance < distanceThreshold) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  }

  delay(500);  // Delay between readings
}
```


---

Let me know if you'd like a **Wokwi link**, **QR code**, or a badge for "Built with ESP32 + Arduino".
