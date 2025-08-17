- ESP32 Wi-Fi
    
- HTTPS firmware update server

### Code
```
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

// Replace with your WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// HTTPS firmware binary URL
const char* firmwareUrl = "https://yourdomain.com/firmware.bin";


const char* fingerprint = "XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX";


const int currentFirmwareVersion = 1;

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected.");

  checkForUpdates();
}

void checkForUpdates() {
  WiFiClientSecure client;
  client.setTimeout(5000);

  
  client.setInsecure();

  HTTPClient https;
  Serial.println("Connecting to update server...");

  if (https.begin(client, firmwareUrl)) {
    int httpCode = https.GET();
    if (httpCode == HTTP_CODE_OK) {
      int contentLength = https.getSize();
      bool canBegin = Update.begin(contentLength);

      if (canBegin) {
        Serial.println("Begin OTA update...");
        size_t written = Update.writeStream(https.getStream());

        if (written == contentLength) {
          Serial.println("Written : " + String(written) + " bytes successfully");
        } else {
          Serial.println("Written only : " + String(written) + "/" + String(contentLength) + " bytes");
        }

        if (Update.end()) {
          if (Update.isFinished()) {
            Serial.println("OTA update finished. Rebooting...");
            ESP.restart();
          } else {
            Serial.println("OTA update not finished.");
          }
        } else {
          Serial.println("Error Occurred. Error #: " + String(Update.getError()));
        }
      } else {
        Serial.println("Not enough space to begin OTA");
      }
    } else {
      Serial.println("HTTP GET failed, error: " + String(httpCode));
    }

    https.end();
  } else {
    Serial.println("HTTPS connection failed.");
  }
}

void loop() {
  
}
```