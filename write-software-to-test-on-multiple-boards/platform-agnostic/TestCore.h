#pragma once
#include <stdint.h>
#include <string>

struct PlatformAPI {
    void (*serial_print)(const char* s);
    uint32_t (*millis)();
    bool (*gpio_write)(int pin, bool value);
    bool (*gpio_read)(int pin, bool &value);
    bool (*i2c_read_reg)(uint8_t addr, uint8_t reg, uint8_t* buf, size_t len);
    bool (*i2c_write_reg)(uint8_t addr, uint8_t reg, const uint8_t* data, size_t len);
};

void run_all_tests(const PlatformAPI* api);
