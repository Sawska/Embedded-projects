#include <LiquidCrystal.h>

// LCD pins: rs, en, d4, d5, d6, d7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// HC-SR04 sensor and alert pins
const int trigPin = 8;
const int echoPin = 9;
const int buzzerPin = 7;
const int ledPin = 6;
const int distanceThreshold = 24;  // Distance alert threshold (cm)

void setup() {
  delay(1000); // Allow LCD to power up
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Distance:");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo time
  long duration = pulseIn(echoPin, HIGH, 30000);  // Timeout = 30ms
  float distance = duration * 0.034 / 2;

  // Debug output
  Serial.print("Distance: ");
  Serial.print(distance, 1);
  Serial.println(" cm");

  // Update LCD (only overwrite what's needed)
  lcd.setCursor(0, 1);
  lcd.print("Dist: ");
  lcd.print(distance, 1);  // 1 decimal point
  lcd.print(" cm   ");     // Padding to erase old digits

  // Trigger alert if object is too close
  if (distance > 0 && distance < distanceThreshold) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  }

  delay(500);
}
