#include "lpc17xx_systick.h"
#include "ioboard/i2c.h"
#include "leds.h"
#include "motors.h"

void main(void) {
    ioboard_i2c_init();
    led_setup();

    SYSTICK_InternalInit(1);
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);

    return;
}


void SysTick_Handler(void) {
    // heartbeat
    static uint8_t led_state = 0;
    static uint8_t led_tick = 0;
    if ((led_tick += 1) > 200) {
        led_clear();
        led_disp_num(led_state ^ 1);
    }


}


