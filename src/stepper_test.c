#include "lpc17xx_i2c.h"
#include "ioboard/i2c.h"
#include "mbed/rit.h"
#include "leds.h"
#include "mbed/wait.h"

#define XYLATCH 0x3e

uint8_t states[4] = {0x09, 0x05, 0x06, 0x0a};
I2C_M_SETUP_Type packet = {
        .sl_addr7bit = XYLATCH,
        .tx_data = states,
        .tx_length = 1
    };

void main(void) {
    ioboard_i2c_init();

    led_setup();
    led_disp_num(0xff);

    mbed_rit_init();
    mbed_rit_set(200e3);
    mbed_rit_state(ENABLE);
    while (1);
}

void RIT_IRQHandler(void) {
    led_clear();
    /*
    I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
    packet.tx_data += 1;
    if (packet.tx_data - states > 3) {
        packet.tx_data = states;
    }
    */
}
