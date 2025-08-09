#include "TestCore.h"
#include <cstdio>

static void print(const PlatformAPI* api, const char* s){
    api->serial_print(s);
}

void run_all_tests(const PlatformAPI* api){
    print(api, "TEST:START\n");

    // --- GPIO loopback test ---
    print(api, "TEST:GPIO:START\n");
    int out_pin = 2; // example
    int in_pin = 3;
    // Set out HIGH then read in
    api->gpio_write(out_pin, true);
    bool v=false;
    if(!api->gpio_read(in_pin, v)){
        print(api, "TEST:GPIO:ERROR:READ_FAIL\n");
    } else if(v){
        print(api, "TEST:GPIO:PASS\n");
    } else {
        print(api, "TEST:GPIO:FAIL\n");
    }

    // --- I2C sensor test ---
    print(api, "TEST:I2C:START\n");
    uint8_t buf[2];
    if(api->i2c_read_reg(0x76, 0xD0, buf, 1)){ // common BMP/BME ID reg
        char line[64];
        sprintf(line, "TEST:I2C:ID:0x%02X\n", buf[0]);
        print(api, line);
        print(api, "TEST:I2C:PASS\n");
    } else {
        print(api, "TEST:I2C:ERROR:COMM\n");
    }

    print(api, "TEST:END\n");
}
