# Embedded Projects

A collection of embedded-systems projects spanning Arduino UNO, ESP32, and Raspberry Pi Pico —
covering peripherals (I2C, SPI, CAN, BLE), RTOS, low-power design, security, and tooling.

Each project lives in its own folder with a `README.md` describing the goal, bill of materials,
and the concepts it demonstrates.

## Projects

### Communication & Connectivity
| Project | Description |
| --- | --- |
| [esp32-bluetooth-remote](esp32-bluetooth-remote/) | Bluetooth remote to control LEDs or a robot wirelessly over ESP32 SerialBT. |
| [use-ble](use-ble/) | Bluetooth Low Energy (BLE) communication on ESP32. |
| [secure-bluetooth-lock](secure-bluetooth-lock/) | ESP32 + FreeRTOS lock with AES-encrypted unlock codes. |
| [can-bus-motor-control](can-bus-motor-control/) | Two ESP32 boards communicating over CAN bus to control a motor. |
| [i2c-multi-sensor-hub](i2c-multi-sensor-hub/) | Arduino UNO as an I2C master reading multiple I2C sensors. |
| [logic-analyzer-for-i2c](logic-analyzer-for-i2c/) | Capture I2C transactions with an ESP32 and PulseView. |
| [secure-ota-update](secure-ota-update/) | Secure over-the-air firmware updates over ESP32 Wi-Fi. |

### Displays & UI
| Project | Description |
| --- | --- |
| [spi-oled-dashboard](spi-oled-dashboard/) | Display sensor data on an SPI OLED (SSD1306). |
| [display-text-on-a-screen-with-a-different-font](display-text-on-a-screen-with-a-different-font/) | Render text on a screen using a custom font. |
| [real-time-clock-alarm-with-lcd](real-time-clock-alarm-with-lcd/) | DS3231 RTC alarm with LCD output. |
| [temperature-humidity-monitor](temperature-humidity-monitor/) | Temperature, humidity, and soil-moisture monitor with an OLED menu. |

### Sensors & Data Logging
| Project | Description |
| --- | --- |
| [multi-sensor-data-logger](multi-sensor-data-logger/) | Log data from multiple sensors to an SD card with timestamps. |
| [encrypted-sensor-logging](encrypted-sensor-logging/) | Raspberry Pi Pico W logging encrypted sensor data to an SD card. |
| [embedded-linux-iot-weather-station](embedded-linux-iot-weather-station/) | Pico W collects weather data; an Embedded Linux Pi shows a dashboard. |
| [motion-activated](motion-activated/) | PIR motion-activated projects (alarm, LCD, basic PIR). |

### Motor & Real-Time Control
| Project | Description |
| --- | --- |
| [real-time-motor-control](real-time-motor-control/) | DC motor speed/direction control with FreeRTOS and encoder feedback. |
| [rtos-based-smart-home-door-lock](rtos-based-smart-home-door-lock/) | FreeRTOS door lock scheduling keypad, RFID, and servo tasks. |
| [state-machine-project](state-machine-project/) | Event-driven control with a clear state machine. |

### Core Peripherals & MCU Concepts
| Project | Description |
| --- | --- |
| [interrupts](interrupts/) | Working with hardware interrupts. |
| [clock-management](clock-management/) | Configuring and managing MCU clocks. |
| [dma-direct-memory-access](dma-direct-memory-access/) | Using DMA for efficient data transfer. |
| [power-management](power-management/) | Power-management techniques on ESP32 / Pico. |
| [mcu-sleep-in-low-power-project](mcu-sleep-in-low-power-project/) | Deep sleep on the ATmega328P with `LowPower.h`. |
| [watchdogs](watchdogs/) | Using the watchdog timer for reliability. |
| [pico-based-usb-hid-device](pico-based-usb-hid-device/) | Raspberry Pi Pico as a USB HID keyboard. |
| [led-manipulation-with-controller](led-manipulation-with-controller/) | Joystick-controlled LED projects (snake game, multicolor control). |

### Data Structures & Algorithms (on-device)
| Project | Description |
| --- | --- |
| [circular-buffer-project](circular-buffer-project/) | Ring buffer for periodic sensor sampling. |
| [circular-buffer-with-more-than-2-pointers](circular-buffer-with-more-than-2-pointers/) | Circular buffer variant using more than two pointers. |
| [run-tests-in-an-algorithm-sandbox](run-tests-in-an-algorithm-sandbox/) | Implement and compare sorting algorithms on limited hardware. |
| [optimise-assembly-code](optimise-assembly-code/) | Hand-optimizing assembly on ESP32. |

### Firmware, Tooling & Testing
| Project | Description |
| --- | --- |
| [bootloader-dfu](bootloader-dfu/) | Bootloader / Device Firmware Update example. |
| [fix-49-day-reboot-problem](fix-49-day-reboot-problem/) | Avoiding `millis()` overflow / 49-day reboot bug. |
| [serial-debug-dashboard](serial-debug-dashboard/) | Stream debug data to the PC serial plotter. |
| [write-manufacturing-test-software](write-manufacturing-test-software/) | Automated functional tests for hardware. |
| [write-software-to-test-on-multiple-boards](write-software-to-test-on-multiple-boards/) | Cross-platform test firmware for Uno, ESP32, and Pico. |

## License

See [LICENSE](LICENSE).
