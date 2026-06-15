#include "TestCore.h"
#include <Arduino.h>
#include <Wire.h>

static void arduino_serial_print(const char* s){ Serial.print(s); }
static uint32_t arduino_millis(){ return millis(); }

static bool arduino_gpio_write(int pin, bool value){
    digitalWrite(pin, value ? HIGH : LOW);
    return true;
}
static bool arduino_gpio_read(int pin, bool &value){
    value = (digitalRead(pin) == HIGH);
    return true;
}
static bool arduino_i2c_read_reg(uint8_t addr, uint8_t reg, uint8_t* buf, size_t len){
    Wire.beginTransmission(addr);
    Wire.write(reg);
    if(Wire.endTransmission(false) != 0) return false;
    size_t r = Wire.requestFrom((int)addr, (int)len);
    if(r < len) return false;
    for(size_t i=0;i<len;i++) buf[i] = Wire.read();
    return true;
}
static bool arduino_i2c_write_reg(uint8_t addr, uint8_t reg, const uint8_t* data, size_t len){
    Wire.beginTransmission(addr);
    Wire.write(reg);
    for(size_t i=0;i<len;i++) Wire.write(data[i]);
    return Wire.endTransmission() == 0;
}

// Example sketch
#include "ArduinoPlatform.h"
PlatformAPI api = {
    arduino_serial_print,
    arduino_millis,
    arduino_gpio_write,
    arduino_gpio_read,
    arduino_i2c_read_reg,
    arduino_i2c_write_reg
};

void setup(){
  Serial.begin(115200);
  Wire.begin(); // for ESP32 you might pass SDA/SCL pins
  pinMode(2, OUTPUT);
  pinMode(3, INPUT_PULLUP); // expect loopback wiring: connect D2 to D3
  delay(1000);
  run_all_tests(&api);
}

void loop(){}
