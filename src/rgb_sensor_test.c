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
    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = RGB_SENSOR_ADDR,
    };

    ioboard_i2c_init();
    serial_init();

    out_len = snprintf(output, 256, "start");
    serial_write_b(output, (uint32_t)out_len);

    // Enable chip
    uint8_t tx_data[] = {0xa0, 0x03};
    packet.tx_data = tx_data;
    packet.tx_length = 2;
    I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);

    // Set address to clear high, (with auto-increment)
    tx_data[0] = 0xb4;
    packet.tx_length = 1;
    I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);

    packet = (I2C_M_SETUP_Type){
        .sl_addr7bit = RGB_SENSOR_ADDR,
        .rx_data = rx_data.low_high,
        .rx_length = 8,
    };

    wait_us(2400);

    while (1) {
        wait_us(integration_time(0xff) + 2400);
        I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
        serial_write_b((char *)rx_data.low_high, 8);
    }
}

uint16_t integration_time(uint8_t atime) {
    return 2400 * (256 - atime);
}
