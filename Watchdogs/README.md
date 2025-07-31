**Project:** **Self-Healing IoT Sensor Node**  
Make an ESP32-based sensor node that automatically resets if the main loop freezes.

**BOM:**

- ESP32
    
- DHT22 sensor
    
- Breadboard
    

**Implementation:**

- Enable **hardware watchdog timer**.
    
- Feed the watchdog in normal operation.
    
- Intentionally cause a hang (e.g., `while(1)` loop) to test auto-reset.
    

**Skills learned:**

- Configuring watchdog
    
- Understanding watchdog timeout behavior
    
- Safe recovery after crash