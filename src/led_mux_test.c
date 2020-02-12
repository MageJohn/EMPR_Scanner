#include "leds.h"
#include "time.h"

void main(void) {
    led_setup();
    time_init();
    struct LedSource *led1 = led_mux_register_source(1);
    struct LedSource *led2 = led_mux_register_source(2);

    led1->num = 1;
    led2->num = 2;

    while (1) {
        led_mux_set_curr(1);
        led_mux_tick();
        wait_ms(500);
        led_mux_set_curr(2);
        led_mux_tick();
        wait_ms(500);
    }
}
