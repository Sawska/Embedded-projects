**Project:** **Battery-Powered Sleep/Wake Sensor**  
Build a temperature sensor that sleeps most of the time to save battery.

**BOM:**

- ESP32 or Raspberry Pi Pico
    
- DHT22 sensor
    
- Battery holder
    

**Implementation:**

- Deep sleep for 10 minutes.
    
- Wake up, read sensor, send data over Wi-Fi.
    
- Return to sleep.
    

**Skills learned:**

- Deep sleep / light sleep
    
- Wake-up timers & GPIO triggers
    
- Measuring battery current