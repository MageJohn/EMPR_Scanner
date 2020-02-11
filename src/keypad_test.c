#include "lpc17xx_gpio.h"
#include "platform_keypad.h"
#include "platform.h"
#include "leds.h"

void main(void) {
    led_setup();
    platform_init();
    platform_keypad_int_init();
    led_mux_set_curr(KP_LED);
}
