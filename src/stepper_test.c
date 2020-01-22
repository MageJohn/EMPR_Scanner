#include "lpc17xx_i2c.h"
#include "lpc17xx_systick.h"
#include "ioboard/i2c.h"
#include "leds.h"

#define XYLATCH 0x3e

uint8_t states[4] = {0x09, 0x05, 0x06, 0x0a};
I2C_M_SETUP_Type packet = {
        .sl_addr7bit = XYLATCH,
        .tx_data = states,
        .tx_length = 1
    };
uint8_t led_state = 0;

void main(void) {
    ioboard_i2c_init();

    led_setup();

    SYSTICK_InternalInit(1);
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);

    return;
}

void SysTick_Handler(void) {
    static uint8_t led_state = 0xff;
    static uint8_t motor_counter = 0;
    motor_counter += 1;
    if (motor_counter > 200) {
        motor_counter = 0;
        led_clear();
        led_disp_num(led_state ^= 0xff);
        I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
        packet.tx_data += 1;
        if ((packet.tx_data - states) > 3) {
            packet.tx_data = states;
        }
    }
}
