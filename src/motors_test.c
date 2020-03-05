#include "platform.h"
#include "leds.h"
#include "time.h"

void main(void) {
    leds_init();
    platform_init();

    leds_mux_set_curr(HB_LED);

    platform_calibrate_head();
    while (!platform_calibrated());
    platform_head_set_coords(500, 500, 100);
}
