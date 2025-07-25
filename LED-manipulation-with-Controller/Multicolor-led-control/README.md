# 🎨 Multicolor LED Control with Joystick and Button – ESP32

### 📌 **Board:** ESP32

### 🔧 **Peripherals:**

* Common Cathode RGB LED (or Common Anode with inverted logic)
* Joystick Module (analog)
* Push Button

---

### 🧠 **Skills Demonstrated**

* PWM control for smooth RGB color mixing
* Analog-to-Digital Conversion (ADC) for joystick input
* Button debounce logic and mode switching (state machine)
* Smooth color transitions using sine-based blending

---

### 📄 **Project Description**

This project demonstrates how to control an RGB LED’s color and behavior using a joystick and a push button on the ESP32. The joystick adjusts the color in real time, while the button toggles between **three modes**:

1. 🎮 **Joystick-Controlled Color Mode** – Control RGB values using the joystick’s X and Y axes.
2. 💡 **Blinking Mode** – RGB LED blinks on and off.
3. 🌈 **Color Cycling Mode** – Smoothly cycles through RGB colors using sine functions.

---

### 🧪 **ESP32 Code**

```cpp
// Pin assignments
const int buttonPin = 26;
const int redPin    = 27;
const int greenPin  = 25;
const int bluePin   = 33;
const int yAxis     = 34; 
const int xAxis     = 35;

int lastState = HIGH;
int currentState;

int mode = 0; 
unsigned long lastChangeTime = 0;
bool ledState = false;
int cycleHue = 0;

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redPin,    OUTPUT);
  pinMode(greenPin,  OUTPUT);
  pinMode(bluePin,   OUTPUT);
}

void loop() {
  currentState = digitalRead(buttonPin);

  // Mode toggle logic
  if (currentState == LOW && lastState == HIGH) {
    mode = (mode + 1) % 3;
    Serial.print("Mode changed to: ");
    Serial.println(mode);
    delay(200); // Debounce
  }
  lastState = currentState;

  switch (mode) {
    case 0: { // Joystick-controlled mode
      int valueX = analogRead(xAxis); 
      int valueY = analogRead(yAxis); 
      Serial.println(String("X: ") + valueX + "  Y: " + valueY);

      int redVal   = map(valueX, 0, 4095, 0, 255);
      int greenVal = map(valueY, 0, 4095, 0, 255);
      int blueVal  = 255 - ((redVal + greenVal) / 2);

      analogWrite(redPin,   redVal);
      analogWrite(greenPin, greenVal);
      analogWrite(bluePin,  blueVal);
      break;
    }

    case 1: { // Blinking mode
      if (millis() - lastChangeTime > 500) {
        ledState = !ledState;
        analogWrite(redPin,   ledState ? 255 : 0);
        analogWrite(greenPin, ledState ? 255 : 0);
        analogWrite(bluePin,  ledState ? 255 : 0);
        lastChangeTime = millis();
      }
      break;
    }

    case 2: { // Color cycling mode
      if (millis() - lastChangeTime > 30) {
        int r = (sin(cycleHue * 0.1) + 1) * 127;
        int g = (sin(cycleHue * 0.1 + 2) + 1) * 127;
        int b = (sin(cycleHue * 0.1 + 4) + 1) * 127;

        analogWrite(redPin,   r);
        analogWrite(greenPin, g);
        analogWrite(bluePin,  b);

        cycleHue = (cycleHue + 1) % 360;
        lastChangeTime = millis();
      }
      break;
    }
  }
}
```

> ⚠️ *On ESP32, analogWrite() is not natively supported. For real applications, replace it with `ledcWrite()` and `ledcAttachPin()`.*

---

### 🔗 **Wokwi Diagram**

<img width="660" alt="Screenshot 2025-06-30 at 08 23 54" src="https://github.com/user-attachments/assets/3574f64c-7497-4369-9dbc-6c7272fc6885" />


[🔌 View the Circuit in Wokwi](https://wokwi.com/projects/434760393490739201)


###  **Real Hardware photo**

![image](https://github.com/user-attachments/assets/568e5a6d-20fc-47e8-923a-a50a1be9d4eb)

### **Real Hardware video**



https://github.com/user-attachments/assets/7cc82535-a645-4ccf-9536-325950f7eab0


- used multimitter to get values 

![image](https://github.com/user-attachments/assets/7db54eee-e9e7-4ed7-9507-af8407338f63)


### 🧠 **PCB Design**
<img width="816" height="579" alt="Screenshot 2025-07-23 at 21 04 14" src="https://github.com/user-attachments/assets/8ae09b52-8455-4d35-9534-e6ae119842a2" />

