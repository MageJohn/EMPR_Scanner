#include "lpc17xx_gpio.h"
#include "platform_keypad.h"
#include "platform.h"
#include "leds.h"

void main(void) {
    leds_init();
    platform_init();
    platform_keypad_int_init();
    leds_mux_set_curr(KP_LED);
}
