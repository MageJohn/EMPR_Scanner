#include "lpc_types.h"


/*------------------------------
 * CONSTANTS
 *------------------------------*/

#define LED_PORT 1
#define LEDS 1<<18|1<<20|1<<21|1<<23


/*------------------------------
 * FUNCTIONS
 *------------------------------*/

// Display a 4bit number on the mbed leds
//
// Returns:
//      void
//
// Parameters:
//      uint8_t num: number to be displayed. Only the lower nibble is used
void led_disp_num(uint8_t num);


// Clear the mbed leds
//
// Returns:
//      void
//
// Parameters:
//      void
void led_clear(void);


// Setup the led pins
//
// Returns:
//      void
//
// Parameters:
//      void
void led_setup(void);
