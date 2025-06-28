#include <WiFi.h>
#include <HTTPClient.h>

// Your Wi-Fi credentials
const char* ssid = "";
const char* password = "";

// Your HTTP logging endpoint (replace with your computer’s local IP)
const char* serverName = "http://172.28.192.57::3000";  // or whatever IP your PC has


// Pin definitions
const int buzzerPin = 13;
const int echoPin = 26;
const int trigPin = 27;
const int ledPin = 14;

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the distance
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Send distance to server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = String(serverName) + "?distance=" + String(distance);

    http.begin(serverPath);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.println("Logged to server: " + String(httpResponseCode));
    } else {
      Serial.println("Failed to log to server");
    }

    http.end();
  } else {
    Serial.println("Wi-Fi Disconnected");
  }

  // Trigger alarm if object is close
  if (distance < 24) {
    tone(buzzerPin, 1000); // Start buzzer tone
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);     // Stop buzzer tone
    delay(100);
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
  }

  delay(100); // Delay between measurements
}
