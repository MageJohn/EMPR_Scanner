#include "platform.h"
#include "leds.h"
#include "time.h"

void main(void) {
    led_setup();
    platform_init();

    led_mux_set_curr(HB_LED);

    platform_calibrate_head();
    while (!platform_calibrated());
    platform_head_set_coords(280, 400, 30);
}
