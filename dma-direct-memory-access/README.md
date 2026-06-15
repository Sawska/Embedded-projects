# **High-Speed ADC Data Logger** (ESP32)

This project demonstrates how to sample an **analog gas sensor** at high speed using the ESP32's **ADC + DMA** capability (via the I2S peripheral) and store the readings in memory without heavy CPU usage.

---

## **Overview**

Using the ESP32's I2S peripheral in **ADC capture mode**, this project continuously samples an analog signal (from a gas sensor) into a DMA buffer.
This technique bypasses CPU-intensive loops, making it ideal for **high-speed, continuous logging**.

---

## **Bill of Materials (BOM)**

* **ESP32** development board
* **Analog gas sensor** (e.g., MQ-2, MQ-135)
* **SD card module** (optional, for logging data to storage)
* Jumper wires & breadboard

---

## **Implementation Details**

* Configure **I2S in ADC mode** to read the analog pin continuously.
* Use **DMA** to store samples into a buffer.
* Process or log the buffer contents.
* Digital pin controls the gas sensor’s D0 pin (enable signal).

---

## **Skills Learned**

* Configuring ESP32's **I2S peripheral for ADC mode**
* Setting up **DMA buffers**
* High-speed **analog signal acquisition**
* Basic **sensor interfacing**

---

## **Wiring**

| Gas Sensor Pin | ESP32 Pin   | Description                |
| -------------- | ----------- | -------------------------- |
| **A0**         | VP (GPIO36) | Analog output to ADC       |
| **D0**         | D5 (GPIO14) | Digital enable/control pin |

---

## **Code Example**

```cpp
#include <Arduino.h>
#include "driver/i2s.h"

#define GAS_SENSOR_ANALOG_PIN ADC1_CHANNEL_0 // GPIO36 (VP)
#define GAS_SENSOR_DIGITAL_PIN 14             // GPIO14 (D5 pin)

#define SAMPLE_RATE     10000   // 10 kHz sampling
#define BUFFER_SIZE     1024    // Number of samples per buffer

const i2s_port_t i2s_num = I2S_NUM_0;
uint16_t adc_buffer[BUFFER_SIZE];

void setup() {
  Serial.begin(9600);

  pinMode(GAS_SENSOR_DIGITAL_PIN, OUTPUT);
  digitalWrite(GAS_SENSOR_DIGITAL_PIN, HIGH); // Enable sensor D0 pin

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = BUFFER_SIZE / 4,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
  i2s_set_adc_mode(ADC_UNIT_1, GAS_SENSOR_ANALOG_PIN);
  i2s_adc_enable(i2s_num);

  Serial.println("Starting ADC DMA capture...");
}

void loop() {
  size_t bytes_read = 0;
  i2s_read(i2s_num, (void*)adc_buffer, sizeof(adc_buffer), &bytes_read, portMAX_DELAY);

  int samples_read = bytes_read / sizeof(uint16_t);
  Serial.printf("Read %d samples\n", samples_read);

  for (int i = 0; i < samples_read; i++) {
    uint16_t raw = adc_buffer[i] & 0x0FFF;
    float voltage = (raw * 3.3f) / 4095.0f;
    Serial.println(voltage, 3);
  }

  delay(1000);
}
```

---

## **Real Hardware Setup**

<img width="480" alt="Gas sensor logger" src="https://github.com/user-attachments/assets/024dfb51-23b7-40ff-9eb9-8154afce1b70" />

