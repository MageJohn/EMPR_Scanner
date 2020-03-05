#include "lpc17xx_gpio.h"
#include "leds.h"

#define LED_PORT 1
#define LEDS (1 << 18 | 1 << 20 | 1 << 21 | 1 << 23)


// Display a 4bit number on the mbed leds
//
// Returns:
//      void
//
// Parameters:
//      uint8_t num: number to be displayed. Only the lower nibble is used
void leds_disp_num(uint8_t num) {
    uint32_t leds = (num & 0x1) << 18 | (num & 0x2) << (20-1) |
                    (num & 0x4) << (21-2) | (num & 0x8) << (23-3);
    GPIO_SetValue(LED_PORT, leds);
}


// Clear the mbed leds
//
// Returns:
//      void
//
// Parameters:
//      void
void leds_clear(void) {
    GPIO_ClearValue(LED_PORT, LEDS);
}


// Setup the led pins
//
// Returns:
//      void
//
// Parameters:
//      void
void leds_init(void) {
    GPIO_SetDir(LED_PORT, LEDS, 1);
}
