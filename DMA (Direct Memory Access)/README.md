**Project:** **High-Speed ADC Data Logger** (ESP32 or Raspberry Pi Pico)  
Sample an analog sensor at high speed and store directly to memory without CPU intervention.

**BOM:**

- ESP32 or Pico
    
- Analog temperature sensor (e.g., LM35)
    
- SD card module
    

**Implementation:**

- Configure ADC + DMA to transfer samples to a buffer.
    
- Save buffer to SD card in chunks.
    

**Skills learned:**

- DMA setup
    
- Buffer management
    
- Continuous high-speed data logging