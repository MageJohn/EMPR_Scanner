#include "leds.h"
#include "time.h"

void main(void) {
    leds_init();
    time_init();
    uint8_t *led1 = leds_mux_register_source(1);
    uint8_t *led2 = leds_mux_register_source(2);

    *led1 = 1;
    *led2 = 2;

    while (1) {
        leds_mux_set_curr(1);
        leds_mux_tick();
        wait_ms(500);
        leds_mux_set_curr(2);
        leds_mux_tick();
        wait_ms(500);
    }
}
