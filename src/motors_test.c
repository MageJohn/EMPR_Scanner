#include "platform.h"
#include "leds.h"
#include "mbed.h"

#define HB_LED 1
#define X_LED 2
#define Y_LED 3
#define Z_LED 4

void main(void) {
    led_setup();
    platform_init();

    led_mux_set_curr(HB_LED);

    platform_calibrate_head();
    while (!platform_calibrated());
    platform_head_set_coords(900, 900, 100);
}
