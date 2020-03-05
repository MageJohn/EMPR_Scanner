#include <stdbool.h>
#include "lpc_types.h"

/*--------------------------
 * Some look-up tables for
 * platform_keypad_use_lut
 *--------------------------*/

/*
  Scancode -> char
*/

extern uint8_t SCANCODE_CHAR_LUT[16];


/*
  Scancode -> LCD data

  This table translates the scancodes into bytes suitable for writing to the LCD
  display.
*/

extern uint8_t SCANCODE_LCD_LUT[16];


/*
  Have the functions in this library pass lut[scancode] instead of a raw
  scancode. Disable this by passing NULL.

  The scancodes returned by default correspond to the position of the keys on
  the keypad. Use this to translate this scancode to a more useful value.

  Above are two useful look-up tables, use them for reference when defining your
  own.

  Parameters:
      uint8_t *lut: Pointer the lut. Expected to have 16 bytes in it.
*/
void platform_keypad_use_lut(uint8_t *lut);

/*
  Poll the keypad for the state of a key.

  If a key is pressed, put its scancode into *scancode. Returns true if
  *scancode was written to.

  If a lut is set, scancode instead will contain lut[scancode].

  Returns:
       bool: true if *scancode was written to. false otherwise.

  Parameters:
       uint8_t *key: pointer to write the key to.
*/
bool platform_keypad_poll_key(uint8_t *key);


/*
  A rate-limted version of platform_keypad_poll_key.

  This function operates nearly the same as platform_keypad_get_key, except if
  it is repeatedly polled while a key is held down, it doesn't report that a
  key is pressed every time. Instead a counter is decremented each time the
  function is called and the key is still pressed, wrapping at the value of
  rate, and a key is put into *key only if the value equals rate. This
  function works best when called repeatedly in a loop.

  If a lut is set, key instead will contain lut[key].

  Returns:
       bool: true if a key was written to *key

  Parameters:
       uint8_t *key: pointer to write the pressed key to, if it's not rate
       limited
       uint16_t rate: limiting value for how often the key repeats when held
       down
*/
bool platform_keypad_poll_key_rl(uint8_t *key, uint16_t rate);

/*
  Initialise the interrupt handler for keypad.
*/
void platform_keypad_int_init(void);
