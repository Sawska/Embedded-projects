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
  // Detect short IR pulse (non-blocking version)
  unsigned long pulseLength = pulseIn(irSensorPin, LOW, 1000); // microseconds

  if (pulseLength > 0) {
    irLastDetected = millis();
    Serial.println("IR signal detected!");
  }

  bool irBlockingActive = millis() - irLastDetected < irDisableDuration;

  // Measure distance
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

  // Alarm logic
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

  delay(1); // allow some breathing room
}



