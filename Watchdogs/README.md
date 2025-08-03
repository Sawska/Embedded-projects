# **Self-Healing IoT Sensor Node (ESP32 + MQ-2 Gas Sensor)**

This project demonstrates how to build a **self-healing IoT sensor node** using an **ESP32** and an **MQ-2 gas sensor**.
The ESP32 is configured with a **hardware watchdog timer** to automatically **reset itself if the main loop freezes**, ensuring continuous operation without manual intervention.

---

## **Bill of Materials (BOM)**

| Component           | Description                            |
| ------------------- | -------------------------------------- |
| **ESP32**           | Microcontroller with Wi-Fi & Bluetooth |
| **MQ-2 Gas Sensor** | Detects flammable gases and smoke      |
| **Breadboard**      | For quick prototyping                  |
| **Jumper Wires**    | For connections                        |

---

## **Implementation Overview**

1. **Hardware Watchdog Timer (WDT)** is enabled to monitor the main program loop.
2. During **normal operation**, the WDT is **fed/reset** regularly to prevent unintended resets.
3. If the main loop **hangs** (e.g., infinite loop, software crash):

   * WDT times out.
   * ESP32 **resets automatically**.
4. A **simulated hang** is introduced in code after 10 seconds to demonstrate auto-reset.

---

## **Skills Learned**

* Configuring and using **ESP32 hardware watchdog**.
* Understanding **watchdog timeout behavior**.
* Designing **self-recovering embedded systems**.
* Reading analog values from an **MQ-2 gas sensor**.

---

## **Circuit Diagram**

| MQ-2 Pin | ESP32 Pin | Description                         |
| -------- | --------- | ----------------------------------- |
| **VCC**  | 3.3V      | Power supply (works with 3.3V)      |
| **GND**  | GND       | Ground                              |
| **AO**   | GPIO34    | Analog output for gas concentration |

---

## **Code**

```cpp
#include <esp_task_wdt.h> // For hardware watchdog

#define MQ2_PIN 34
#define WDT_TIMEOUT 5 // seconds

void setup() {
  Serial.begin(115200);

  // Create WDT configuration
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = WDT_TIMEOUT * 1000, // milliseconds
    .idle_core_mask = (1 << portNUM_PROCESSORS) - 1, // Watch all cores
    .trigger_panic = true // Reset if WDT times out
  };

  // Initialize hardware watchdog
  esp_task_wdt_init(&wdt_config);
  esp_task_wdt_add(NULL); // Add current thread to WDT watch list

  pinMode(MQ2_PIN, INPUT);

  Serial.println("Self-Healing IoT Sensor Node with MQ-2");
}

void loop() {
  // Feed the watchdog
  esp_task_wdt_reset();

  // Read MQ-2 sensor value
  int gasValue = analogRead(MQ2_PIN);
  Serial.print("Gas Sensor Reading: ");
  Serial.println(gasValue);

  delay(1000);

  // Simulate a hang after 10 seconds
  static unsigned long startTime = millis();
  if (millis() - startTime > 10000) {
    Serial.println("Simulating hang...");
    while (1) {
      // No WDT reset → ESP32 will auto-reset
    }
  }
}
```

---

## **Demonstration Photo**

<img width="804" height="242" alt="Screenshot 2025-08-03 at 14 03 21" src="https://github.com/user-attachments/assets/6db55e89-6bbc-41ca-9bd5-dd1c6e41a6f7" />

---

## **How It Works**

1. The ESP32 **feeds** the watchdog during normal operation.
2. If the loop **freezes**, the watchdog times out.
3. The ESP32 **resets itself** automatically without user intervention.
4. This ensures **uninterrupted monitoring** of gas levels even in case of software issues.

---

