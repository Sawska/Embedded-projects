### **Project 1 — Multi-Sensor Data Logger**

**Description:**  
Collect and log data from multiple sensors (temperature, light, motion) to an SD card with timestamp.

**BOM:**

- Arduino UNO
    
- DHT22 sensor
    
- LDR (light sensor)
    
- PIR motion sensor
    
- SD card module
    
- RTC module (DS3231)
    

**Scope Mapping:**

- **GPIO / ADC:** LDR reading.
    
- **I2C / SPI:** DS3231 RTC & SD card.
    
- **Timers:** Time-based logging.