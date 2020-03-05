#include "lpc17xx_systick.h"
#include "lpc17xx_clkpwr.h"

#include "leds.h"
#include "time.h"
#include "motors.h"
#include "sensor.h"
#include "platform_i2c.h"
#include "platform.h"

static uint8_t *heartbeat;
static uint32_t hb_start = 0;

void platform_init(void) {
    leds_init();
    i2c_init();
    motors_init();
    time_init();

    heartbeat = leds_mux_register_source(HB_LED);
    leds_mux_set_curr(HB_LED);

    SYSTICK_InternalInit(1);
    SysTick->LOAD = (SystemCoreClock/10e6) * 700 - 1;
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);

    sensor_init();
}

void SysTick_Handler(void) {
    // heartbeat
    if (millis() - hb_start > 200) {
        hb_start = millis();
        *heartbeat ^= 1;
    }

    leds_mux_tick();
    motors_tick();
    i2c_tick();
}
