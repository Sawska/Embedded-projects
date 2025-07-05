# 🌡️ Temperature & Humidity Monitor with OLED Display

### 🧰 Board:

ESP32 or Raspberry Pi Pico

### 📦 Sensors & Peripherals:

* DHT11 or LM35 (Temperature)
* Soil Moisture Sensor (Analog)
* 0.96" OLED Display (I2C - SSD1306)
* Push Buttons or Joystick for Navigation

---

### 💡 Project Description

This project reads **temperature**, **humidity**, and **soil moisture** data using a combination of digital and analog sensors. It displays the values on a 0.96-inch OLED screen with a simple menu interface. Menu navigation is handled using two push buttons (or optionally a joystick). Perfect for indoor gardening, greenhouse monitoring, or environmental sensing projects.



---

### 🔌 Pin Configuration (ESP32)

| Component          | ESP32 Pin      |
| ------------------ | -------------- |
| DHT11 Signal       | GPIO 4         |
| Soil Sensor (AOUT) | GPIO 34 (ADC1) |
| OLED (SDA)         | GPIO 21        |
| OLED (SCL)         | GPIO 22        |
| Button 1 (Menu)    | GPIO 18        |
| Button 2 (Select)  | GPIO 19        |

---

### 🧠 Skills Demonstrated

* Digital and analog sensor reading
* I2C communication
* OLED display handling (SSD1306)
* Basic UI design with menu navigation
* Debouncing and button logic

---

### 🧠  Wokwi Diagram

partially not complete,due to the fact that Wokwi doens't provide needed sensors 
<img width="466" alt="Screenshot 2025-07-01 at 13 39 46" src="https://github.com/user-attachments/assets/50ebab61-69fd-4474-827c-b63cddc0e0b8" />


### 🧾 Code Example (Arduino IDE for ESP32)

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Constants
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

#define DHTPIN 4
#define DHTTYPE DHT11
#define SOIL_PIN 34
#define BUTTON_MENU 18
#define BUTTON_SELECT 19

// Objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

// Menu
int menuIndex = 0;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 200;

// Setup
void setup() {
  pinMode(BUTTON_MENU, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(SOIL_PIN, INPUT);

  Serial.begin(115200);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED failed"));
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

// Read soil moisture (analog value)
int readSoilMoisture() {
  return analogRead(SOIL_PIN);  // usually ranges 0–4095
}

// Display functions
void showTemperature() {
  float temp = dht.readTemperature();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Temperature:");
  display.print(temp);
  display.println(" C");
  display.display();
}

void showHumidity() {
  float hum = dht.readHumidity();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Humidity:");
  display.print(hum);
  display.println(" %");
  display.display();
}

void showSoil() {
  int soil = readSoilMoisture();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Soil Moisture:");
  display.print(map(soil, 0, 4095, 100, 0)); // Inverted % moisture
  display.println(" %");
  display.display();
}

void showMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Select Option:");
  display.setCursor(10, 16);
  display.println(menuIndex == 0 ? "> Temp" : "  Temp");
  display.setCursor(10, 32);
  display.println(menuIndex == 1 ? "> Humidity" : "  Humidity");
  display.setCursor(10, 48);
  display.println(menuIndex == 2 ? "> Soil" : "  Soil");
  display.display();
}

// Loop
void loop() {
  showMenu();

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (digitalRead(BUTTON_MENU) == LOW) {
      menuIndex = (menuIndex + 1) % 3;
      lastDebounceTime = millis();
    }

    if (digitalRead(BUTTON_SELECT) == LOW) {
      lastDebounceTime = millis();
      switch (menuIndex) {
        case 0: showTemperature(); break;
        case 1: showHumidity(); break;
        case 2: showSoil(); break;
      }
      delay(2000); // View result
    }
  }
}
```

---

### Real photo
![image](https://github.com/user-attachments/assets/eb087115-3b2d-4744-84a4-7d6216c81c7d)

