

# 🚨 Motion-Activated Security Alert System

A simple embedded project using **Arduino Uno** and **ESP32**, designed to trigger an alert when motion is detected. Ideal for learning basic sensor integration, alarm systems, and GPIO control.

---

## 🔧 Features

* **Microcontrollers**: Arduino Uno & ESP32
* **Sensors & Actuators**:

  * HC-SR501 PIR Motion Sensor
  * HC-SR04 Ultrasonic Distance Sensor
  * Buzzer (for audible alert)
  * Red LED (for visual alert)
* **Core Skills**:

  * GPIO Input/Output
  * Sensor integration
  * Alarm logic
  * Delay and reset logic
  * (Bonus) IR Remote to enable/disable system





---

## 📋 Description

When motion or proximity is detected (closer than 24 cm), the **buzzer sounds** and the **LED flashes**. The system then resets after a short delay, waiting for the next detection.

> 💡 *Bonus Idea*: Extend functionality using an **IR remote** to toggle the system ON/OFF (e.g., for arming/disarming the alarm).

---

## 🧩 Arduino Uno Implementation

### 🧭 Circuit Diagram

> [View the interactive Wokwi simulation](https://wokwi.com/projects/434304672270491649)
>
> <img width="650" alt="Wokwi Simulation Screenshot" src="https://github.com/user-attachments/assets/b4887193-775f-4bdb-9eda-69f995e289a2" />

---

### 📸 Real Hardware Setup

![image](https://github.com/user-attachments/assets/2def476c-1fe7-4111-8c95-f5d46bc559b0)


---

### 📸 Video



https://github.com/user-attachments/assets/5dff4163-5a94-4c26-81d2-3e2939967918


---

### 🧠 Arduino Code

```cpp
const int buzzerPin = 12;
const int echoPin = 10;
const int trigPin = 9;      
const int ledPin = 11;

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure distance
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Display distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Trigger alarm if object is close
  if (distance < 24) {
    tone(buzzerPin, 1000);
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
    delay(100);
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
  }

  delay(100); // Small delay for stability
}
```

---

### 💬 Code Explanation

* **Ultrasonic Sensor (HC-SR04)**:

  * Sends out a sound wave using `trigPin`.
  * Waits for the echo on `echoPin`.
  * Calculates distance based on time delay.

* **LED + Buzzer**:

  * If an object is within **24 cm**, the LED flashes and buzzer sounds.
  * System resets every cycle to re-check the distance.

---

### Adding IR remote

#### Circuit Diagram

<img width="564" alt="Screenshot 2025-06-21 at 18 32 15" src="https://github.com/user-attachments/assets/8effba2d-ff74-4cc3-822c-c4a3a8ed0ef4" />

#### code

due to innability to add IrReceiver library that turns IR receiver into button or switch, i just checked weither IR receiver gets any signal at all

```cpp
const int buzzerPin = 12;
const int echoPin = 10;
const int trigPin = 9;
const int ledPin = 11;
const int irSensorPin = 2;

const int alarmThreshold = 24;

long duration;
int distance;
unsigned long irLastDetected = 0;
const unsigned long irDisableDuration = 5000;
unsigned long lastBlinkTime = 0;
bool ledState = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(irSensorPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  
  if (digitalRead(irSensorPin) == LOW) {
    irLastDetected = millis();
    Serial.println("IR signal detected!");
  }

  
  bool irBlockingActive = millis() - irLastDetected < irDisableDuration;

  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 30000);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  
  if (distance > 0 && distance < alarmThreshold && !irBlockingActive) {
    
    if (millis() - lastBlinkTime >= 250) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      if (ledState)
        tone(buzzerPin, 1000);
      else
        noTone(buzzerPin);
      lastBlinkTime = millis();
    }
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
  }

  
  delay(5);
}

```

#### 📸 Real Hardware Setup

![image](https://github.com/user-attachments/assets/b6cadccb-ca34-4e62-a12d-9202d7197229)



### 🧩 PIR motion sensor

#### 🧠 Arduino Code

``` cpp
int ledPin = 2;
int buzzerPin = 3;
int PipSensor = 4;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(PipSensor, INPUT);
  Serial.begin(9600); // Optional: to monitor sensor values
}

void loop() {
  int sensorValue = digitalRead(PipSensor);
  Serial.println(sensorValue); // Optional: for debugging

  if (sensorValue == HIGH) { // Motion detected
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }

  delay(100); // Small delay to debounce sensor reading
}
```

#### 📸 Real Hardware Setup

![image](https://github.com/user-attachments/assets/6cc3df45-6820-4b63-a945-65caf6131df2)


#### 📸 Video




https://github.com/user-attachments/assets/8653999c-3e98-472d-99a9-eff06f94bba3





## ESP32 implementation 


### 📸 Real Hardware Setup

![image](https://github.com/user-attachments/assets/f66fc32b-a1b1-46c6-af7c-549cfc8e2d3b)


### 📸 Video




https://github.com/user-attachments/assets/42995d28-3d5e-4cb0-acf3-51b91b22d82d



### 🧠 ESP32 code 

```cpp
const int buzzerPin = 13;
const int echoPin = 27;
const int trigPin = 14;      
const int ledPin = 26;

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure distance
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Display distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Trigger alarm if object is close
  if (distance < 24) {
    tone(buzzerPin, 1000);
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
    delay(100);
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
  }

  delay(100); // Small delay for stability
}
```



## 🧩 TODO / Extensions

* [x] Add IR remote to enable/disable the system.
* [x] Replace distance sensor with PIR motion sensor (HC-SR501).
* [x] Upload version for ESP32.
* [x] Add notification or logging feature (e.g., via Wi-Fi with ESP32).

---


- used multimiter to get values
![image](https://github.com/user-attachments/assets/33d77950-9391-411c-8f17-fbb5d9a4d692)


