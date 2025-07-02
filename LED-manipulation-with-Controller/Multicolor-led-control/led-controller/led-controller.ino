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