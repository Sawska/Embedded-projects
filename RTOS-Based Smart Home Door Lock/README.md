### **Project 1 — RTOS-Based Smart Home Door Lock**

**Description:**  
Create a smart home door lock using FreeRTOS with task scheduling for keypad input, RFID reading, and servo motor control.

**BOM:**

- ESP32 (better Wi-Fi support & FreeRTOS built-in)
    
- 4x4 Keypad module
    
- RFID module (RC522)
    
- Servo motor (SG90 or MG90S)
    
- Buzzer
    
- Breadboard + jumper wires
    

**Scope Mapping:**

- **Real-Time OS:** FreeRTOS tasks for keypad scan, RFID reading, and motor control.
    
- **GPIO / PWM:** Servo control via PWM.
    
- **Interrupts:** Button interrupt for emergency unlock.