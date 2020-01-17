#include "leds.h"
#include "serial.h"

void led_code_enable(void) {
    led_setup();
}

void led_code(uint8_t code) {
    led_clear();
    led_disp_num(code);
}
