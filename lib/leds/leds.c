#include "lpc17xx_gpio.h"
#include "lpc_types.h"
#include "leds.h"
#include <stdio.h>
#include "lpc17xx_clkpwr.h"


// Display a 4bit number on the mbed leds
//
// Returns:
//      void
//
// Parameters:
//      uint8_t num: number to be displayed. Only the lower nibble is used
void led_disp_num(uint8_t num) {
    uint32_t leds = (num & 0x1)<<18|(num & 0x2)<<(20-1)|
                    (num & 0x4)<<(21-2)|(num & 0x8)<<(23-3);
    GPIO_SetValue(LED_PORT, leds);
}


// Clear the mbed leds
//
// Returns:
//      void
//
// Parameters:
//      void
void led_clear(void) {
    GPIO_ClearValue(LED_PORT, LEDS);
}


// Setup the led pins
//
// Returns:
//      void
//
// Parameters:
//      void
void led_setup(void) {
    GPIO_SetDir(LED_PORT, LEDS, 1);
}
