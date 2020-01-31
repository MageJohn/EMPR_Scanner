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
    serial_write_b(output, out_len);

    // This chip support I2C fast mode, up to 400 kHz
    I2C_Init(LPC_I2C1, 400000);

    // Enable chip
    uint8_t tx_data[] = {0xa0, 0x03, 0x00};
    packet.tx_data = tx_data;
    packet.tx_length = 3;
    I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);

    // Set address to clear high, (with auto-increment)
    tx_data[0] = 0xb4;
    packet = (I2C_M_SETUP_Type){
        .sl_addr7bit = RGB_SENSOR_ADDR,
        .tx_data = tx_data,
        .tx_length = 1,
        .rx_data = rx_data.low_high,
        .rx_length = 8,
    };

    wait_us(2400);

    while (1) {
        wait_us(integration_time(0x00) + 2400);
        I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
        //out_len = snprintf(output, 256, "\rC %4d R %4d G %4d B %4d", 
        //        rx_data.combined[0] & 0x7FF,
        //        rx_data.combined[1] & 0x7FF,
        //        rx_data.combined[2] & 0x7FF,
        //        rx_data.combined[3] & 0x7FF);
        //serial_write_b(output, out_len);
        serial_write_b((char *)rx_data.low_high, 8);
    }
}

uint16_t integration_time(uint8_t atime) {
    return 2400 * (256 - atime);
}
