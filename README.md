### ESP32 Bluetooth Remote
**Description:**  
Create a Bluetooth remote to control LEDs or a robot wirelessly.

**BOM:**

- ESP32
    
- LED or motor driver + motor
    
- Smartphone with Bluetooth terminal app
    

**Scope Mapping:**

- **Bluetooth:** Serial communication.
    
- **PWM:** LED brightness or motor speed.

### Code
```
#include "BluetoothSerial.h" // Built-in ESP32 Bluetooth library

BluetoothSerial SerialBT;

const int ledPin = 5; // GPIO5
int brightness = 0;   // PWM value (0-255)

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_LED_REMOTE"); // Bluetooth device name
  pinMode(ledPin, OUTPUT);
  ledcAttachPin(ledPin, 0);           // Attach pin to channel 0
  ledcSetup(0, 5000, 8);              // 5kHz PWM, 8-bit resolution
  Serial.println("Bluetooth device is ready to pair");
}

void loop() {
  if (SerialBT.available()) {
    char command = SerialBT.read();

    switch (command) {
      case '0' ... '9':
        // Map '0' to brightness 0, '9' to 255
        brightness = map(command - '0', 0, 9, 0, 255);
        ledcWrite(0, brightness);
        SerialBT.print("Set brightness to: ");
        SerialBT.println(brightness);
        break;

      case 'x':
        ledcWrite(0, 0); // Turn off LED
        SerialBT.println("LED turned OFF");
        break;

      case 'f':
        ledcWrite(0, 255); // Full brightness
        SerialBT.println("LED at FULL brightness");
        break;

      default:
        SerialBT.println("Unknown command");
        break;
    }
  }
}
```