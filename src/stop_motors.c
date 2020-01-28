#define XYLATCH 0x3E
#define ZPENLATCH 0x3F

#include "ioboard.h"
#include "lpc17xx_i2c.h"

void main() {
    uint8_t tx_data = 0x00;
    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = XYLATCH,
        .tx_length = 1,
        .tx_data = &tx_data,
    };
    ioboard_i2c_init();
    I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
    packet.sl_addr7bit = ZPENLATCH;
    I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
}
