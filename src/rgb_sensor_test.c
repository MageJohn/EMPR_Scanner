#include <stdio.h>
#include "ioboard.h"
#include "mbed.h"
#include "serial.h"

#define RGB_SENSOR_ADDR 0x29

union ColourData {
    uint16_t combined[4];
    uint8_t low_high[8];
};

uint16_t integration_time(uint8_t integ_cycles);

void main(void) {
    char output[256];
    int out_len;
    union ColourData rx_data;

    ioboard_i2c_init();
    ioboard_i2c_address(RGB_SENSOR_ADDR);

    serial_init();

    out_len = snprintf(output, 256, "start");
    serial_write_b(output, (uint32_t)out_len);

    // Enable chip
    ioboard_i2c_write((uint8_t[]){0xa0, 0x03}, 2);

    // Set address to clear high, (with auto-increment)
    ioboard_i2c_write_byte(0xb4);

    wait_us(2400);

    while (1) {
        wait_us(integration_time(0xff) + 2400);
        ioboard_i2c_read(rx_data.low_high, 8);
        serial_write_b((char *)rx_data.low_high, 8);
    }
}

uint16_t integration_time(uint8_t atime) {
    return 2400 * (256 - atime);
}
