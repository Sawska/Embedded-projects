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
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  
  duration = pulseIn(echoPin, HIGH);

  
  distance = duration * 0.034 / 2;

  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  
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

  delay(100); 
}
