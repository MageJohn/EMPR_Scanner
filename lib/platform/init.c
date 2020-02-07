#include "lpc17xx_systick.h"

#include "leds.h"
#include "mbed.h"
#include "motors.h"
#include "sensor.h"
#include "platform_i2c.h"
#include "platform.h"

static struct LedSource* heartbeat;
static uint32_t hb_start = 0;

void platform_init(void) {
    i2c_init();
    motors_init();
    sensor_init();
    timer_init();

    heartbeat = led_mux_register_source(HB_LED);

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
    i2c_tick();
}
