#include <stdbool.h>

/*------------------------------
 * CONSTANTS
 *------------------------------*/

#define KEYPAD_ADDR 0x21
#define NKEYS 16


/*------------------------------
 * FUNCTIONS
 *------------------------------*/

// Poll the keyboard for pressed keys
//
// Returns true if a key was pressed, false otherwise. Also set the
// pressed_keys array to contain the state of the keyboard.
//
// Returns:
//      bool: true if any key was pressed, false otherwise
//
// Parameters:
//      void
bool ioboard_keypad_poll(void);


// Get a key from the keypad, if one is pressed
//
// If a key is pressed, put its scancode into *pressed. Returns true if
// *pressed contains a key.
//
// Returns:
//      bool: true if *pressed was filled. false otherwise
//
// Parameters:
//      uint8_t *pressed: pointer to write the pressed key to
bool ioboard_keypad_get_key(uint8_t *pressed);


// Get a key from the keypad, but if the key is held down limit repeated reads
//
// This function operates nearly the same as ioboard_keypad_get_key, except if
// it is repeatedly polled while a key is held down, it doesn't report that a
// key is pressed every time. Instead a counter is decremented each time the
// function is called and the key is still pressed, wrapping at the value of
// rate, and a key is put into *pressed only if the value equals rate. This
// function assumes it's being called repeatedly in a loop. 
//
// Returns:
//      bool: true if a key was written to *pressed
//
// Parameters:
//      uint8_t *pressed: pointer to write the pressed key to, if it's not rate
//      limited
//      uint16_t rate: limiting value for how often the key repeats when held
//      down
bool ioboard_keypad_rl_get_key(uint8_t *pressed, uint16_t rate);
