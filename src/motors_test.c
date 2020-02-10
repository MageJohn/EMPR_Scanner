#include "platform.h"
#include "leds.h"
#include "mbed.h"

void main(void) {
    led_setup();
    platform_init();

    led_mux_set_curr(HB_LED);

    platform_calibrate_head();
    while (!platform_calibrated());
    platform_head_set_coords(0, 100, 100);
}
