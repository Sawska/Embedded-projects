# Write software to test on multiple boards

**→ Project: Cross-platform GPIO/Sensor Test Tool**

**Goal:** Write reusable test firmware for GPIO/sensor on Uno, ESP32, and Pico.

- **Boards:** All (Arduino Uno, ESP32, Pico)
    
- **Steps:**
    
    - Choose a common task (e.g., I2C sensor read or GPIO toggle).
        
    - Write platform-agnostic logic with board-specific drivers.
        
    - Build and test on all three boards.
        
- **Learning:** Hardware abstraction, testing consistency, CI for embedded.