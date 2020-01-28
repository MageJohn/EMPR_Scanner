#include <stdbool.h>
#include "lpc17xx_i2c.h"

/*------------------------------
 * CONSTANTS
 *------------------------------*/

#define KEYPAD_ADDR 0x21
#define NKEYS 16

#define LCD_ADDR 0x3b
#define DDRAM_SIZE 80

/*------------------------------
 * FUNCTIONS
 *------------------------------*/

// Initialise the i2c bus to which the ioboard peripherals attached
//
// Returns:
//      void
//
// Parameters:
//      void
void ioboard_i2c_init(void);


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

// Initialise the LCD display, ready for writing to
//
// Returns: 
//      void
//
// Parameters:
//      void
void ioboard_lcd_init(void);


// Clears the display
//
// Useful for the R character set, whose Clear_display function is broken
// There is no need to wait for 165 clock cycles
//
// Returns: 
//      void
//
// Parameters:
//      void
void ioboard_lcd_clear_display(void);


// Transmit the data to the LCD display
// 
// Returns: 
//      void
//
// Parameters: 
//      uint8_t *data: Pointer to the data to transmit
//      uint32_t length: Length of the data to transmit
void ioboard_lcd_send_bytes(uint8_t *data, uint32_t length);


// Write arbitrary bytes to the DDRAM
//
// Returns: 
//      void
//
// Parameters:
//      uint8_t *bytes: Pointer to the byte array to write
//      uint8_t length: Length of the byte array to write
//      uint8_t ddram_addr: DDRAM address to write the bytes to
void ioboard_lcd_write_bytes(uint8_t *bytes, uint8_t length, uint8_t ddram_addr);


// Write an ascii string to the display
//
// Takes a null terminated ascii string and translates it as closely as
// possible into the character set of the display driver.  Not all ascii
// characters are supported, and there is no support for the special characters
// supported by the driver. Unsupported characters will be replaced with an
// upside down ?. The advantage is that an ascii string literal can be passed.
// The maximum length of the string is the size of the DDRAM, 80 characters,
// and a longer string will be truncated.
//
// Returns: 
//      void
//
// Parameters:
//      char *string: pointer to the null terminated string to write
//      uint8_t ddram_addr: ddram address to start writing from
void ioboard_lcd_write_ascii(char *string, uint8_t ddram_addr);

