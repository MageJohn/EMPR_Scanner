#include "lpc17xx_systick.h"

#include "leds.h"
#include "mbed.h"
#include "ioboard.h"
#include "motors.h"
#include "platform.h"

struct LedSource* heartbeat;
static uint32_t hb_start = 0;

void platform_init(void) {
    ioboard_i2c_init();
    motors_init();

    heartbeat = led_mux_register_source(1);

    timer_init();

    SYSTICK_InternalInit(1);
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);
}

void SysTick_Handler(void) {
    // heartbeat
    if (millis() - hb_start > 200) {
        hb_start = millis();
        heartbeat->num ^= 1;
    }

    led_mux_tick();
    motors_tick();
}
