#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <cstdio>

static void pico_serial_print(const char* s){ printf("%s", s); }
static uint32_t pico_millis(){ return to_ms_since_boot(get_absolute_time()); }

static bool pico_gpio_write(int pin, bool value){
    gpio_put(pin, value);
    return true;
}
static bool pico_gpio_read(int pin, bool &value){
    value = gpio_get(pin);
    return true;
}
static bool pico_i2c_read_reg(uint8_t addr, uint8_t reg, uint8_t* buf, size_t len){
    // write register
    if(i2c_write_blocking(i2c0, addr, &reg, 1, true) < 0) return false;
    int r = i2c_read_blocking(i2c0, addr, buf, len, false);
    return r == (int)len;
}
