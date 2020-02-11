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

// Set the desired address to work with on i2c
//
// Returns:
//      void
//
// Parameters:
//      i2c address
void ioboard_i2c_address(uint8_t address);


// Write a byte using i2c bus
//
// Returns:
//      SUCCESS if write was performed
//      FAIL if write failed
//
// Parameters:
//      data - data byte to write

Status ioboard_i2c_write_byte(uint8_t data);


// Write data using i2c bus
//
// Returns:
//      SUCCESS if write was performed
//      FAIL if write failed
//
// Parameters:
//      data - A pointer to the data to write
//      length - The number of bytes to write

Status ioboard_i2c_write(uint8_t *data, uint32_t length);


// Read a byte from i2c bus
//
// Returns:
//      SUCCESS if read was performed
//      FAIL if read failed
//
// Parameters:
//      variable - variable to read data into

Status ioboard_i2c_read_byte(uint8_t *variable);


// Read data from i2c bus
//
// Returns:
//      SUCCESS if read was performed
//      FAIL if read failed
//
// Parameters:
//      buffer - a pointer to a buffer to read data into
//      length - the size of the buffer in bytes

Status ioboard_i2c_read(uint8_t *buffer, uint32_t length);


// Read byte from a register over i2c
//
// Write a register address and then read a byte.
//
// Returns:
//      SUCCESS if read was performed
//      FAIL if read failed
//
// Parameters:
//      reg - the register to read from
//      variable - a pointer to store the byte to

Status ioboard_i2c_read_register_byte(uint8_t reg, uint8_t *variable);


// Read data from a register over i2c
//
// Write a register address and then read.
//
// Returns:
//      SUCCESS if read was performed
//      FAIL if read failed
//
// Parameters:
//      reg - the register to read from
//      buffer - a pointer to store the data to
//      length - the size of the buffer in bytes

Status ioboard_i2c_read_register(uint8_t reg, uint8_t *buffer, uint32_t length);


// Combined protocol- first write data, after which read will be performed
//
// Returns:
//      SUCCESS if read and write was performed
//      FAIL if read and write failed
//
// Parameters:
//      write_data - data to write
//      write_length - size of data in bytes
//      read_data - buffer to read data into
//      read_length - the size of the buffer in bytes

Status ioboard_i2c_combined(uint8_t *read_data, uint8_t *write_data,
                         uint32_t read_length, uint32_t write_length);

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


/*------------------------------
 * LCD INSTRUCTION MACROS
 *------------------------------*/

/* control byte macro */
#define Control_byte(CO, RS) (0x00 | ((CO & 1) << 7) | ((RS & 1) << 6))

/* Basic instruction macros */
#define NOP 0x00
#define Function_set(DL, M, SL, H) (0x20 | ((DL & 1) << 4) | ((M & 1) << 2) | ((SL & 1) << 1) | (H & 1))

/* Standard instruction macros */
#define Clear_display 0x01
#define Return_home 0x02
#define Entry_mode_set(I_D, S) (0x40 | ((I_D & 1) << 1) | (S & 1))
#define Display_ctl(D, C, B) (0x08 | ((D & 1) << 2) | ((C & 1) << 1) | (B & 1))
#define Curs_disp_shift(SC, RL) (0x10 | ((SC & 1) << 3) | ((RL & 1) << 2))
#define Set_CGRAM(ACG) (0x40 | (ACG & 0x3f))
#define Set_DDRAM(ADD) (0x80 | (ADD & 0x7f))

/* Extended instruction macros */
#define Screen_conf(L) (0x02 | (L & 1))
#define Disp_conf(P, Q) (0x04 | ((P & 1) << 1) | (Q & 1))
#define Icon_ctl(IM, IB, DM) (0x08 | ((IM & 1) << 2) | ((IB & 1) << 1) | (DM & 1))
#define Temp_ctl(TC1, TC2) (0x10 | ((TC1 & 1) << 1) | (TC2 & 1))
#define HV_gen(S1, S0) (0x40 | ((S1 & 1) << 1) | (S0 & 1))
#define VLCD_set(V, Vx) (0x80 | ((V & 1) << 6) | (Vx & 0x3f))
