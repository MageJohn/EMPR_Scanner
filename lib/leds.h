#include "lpc_types.h"

/*------------------------------
 * STRUCTS
 * -----------------------------*/

struct LedSource {
    uint8_t num;
    uint32_t id;
    struct LedSource *next;
};


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


// Register a new source for the led multiplexer
//
// Returns:
//      A pointer to the new LedSource
//
// Parameters:
//      uint32_t id: The id of the new source.
struct LedSource* led_mux_register_source(uint32_t id);


// Set the current source for the led multiplexer
//
// Returns:
//      void
//
// Parameters:
//      uint32_t id: The idea of a source, as set in a registered LedSource
void led_mux_set_curr(uint32_t id);


// Update the leds with the value of the current source
//
// Returns:
//      void
//
// Parameters:
//      void
void led_mux_tick();
