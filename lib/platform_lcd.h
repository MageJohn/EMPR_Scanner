#include "lpc_types.h"

/*
  Minimum setup:

  #include "platform.h"
  #include "platform_lcd.h"

  void main(void) {
      platform_init();
      platform_lcd_init();
      platform_lcd_clear_display();
  }
*/

/*
Initialise the LCD display, ready for writing to

Returns:
     void

Parameters:
     void
*/
void platform_lcd_init(void);


/*
Clears the display

Useful for the R character set, whose Clear_display function is broken
There is no need to wait for 165 clock cycles

Returns:
     void

Parameters:
     void
*/
void platform_lcd_clear_display(void);


/*
Transmit the data to the LCD display

Returns:
     void

Parameters:
     uint8_t *data: Pointer to the data to transmit
     uint32_t length: Length of the data to transmit
*/
void platform_lcd_send_bytes(uint8_t *data, uint32_t length);


/*
Write arbitrary bytes to the DDRAM

Returns:
     void

Parameters:
     uint8_t *bytes: Pointer to the byte array to write
     uint8_t length: Length of the byte array to write
     uint8_t ddram_addr: DDRAM address to write the bytes to
*/
void platform_lcd_write_bytes(uint8_t *bytes, uint8_t length, uint8_t ddram_addr);


/*
Write an ascii string to the display

Takes a null terminated ascii string and translates it as closely as
possible into the character set of the display driver.  Not all ascii
characters are supported, and there is no support for the special characters
supported by the driver. Unsupported characters will be replaced with an
upside down ?. The advantage is that an ascii string literal can be passed.
The maximum length of the string is the size of the DDRAM, 80 characters,
and a longer string will be truncated.

Returns:
     void

Parameters:
     char *string: pointer to the null terminated string to write
     uint8_t ddram_addr: ddram address to start writing from
*/
void platform_lcd_write_ascii(char *string, uint8_t ddram_addr);


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

