#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "platform.h"
#include "platform/platform_i2c.h"
#include "platform_lcd.h"

#define LEN(x)  (sizeof(x) / sizeof((x)[0]))
#define LCD_ADDR 0x3b
#define DDRAM_SIZE 80

static uint8_t min(uint8_t, uint8_t);


void platform_lcd_init(void) {
    uint8_t init_data[] = {
        Control_byte(0, 0),
        Function_set(1, 1, 0, 0), // set basic instruction set
        Display_ctl(1, 0, 0),     // enable display, disable cursor and blink
        Entry_mode_set(1, 0),     // set incrementing addr, disable disp shift
        Function_set(1, 1, 0, 1), // set extended instruction set
        Disp_conf(0, 0),          // left to right, top to bottom
        Temp_ctl(0, 0),           // temperature coefficient
        HV_gen(1, 0),             // voltage multiplier
        VLCD_set(0, 0x1f),        // LCD supply voltage
        Function_set(1, 1, 0, 0), // set basic instruction set again
        Return_home               // Set DDRAM addr to 0 and reset a shifted display
    };

    I2C_M_SETUP_Type lcd_write = {
        .sl_addr7bit = LCD_ADDR,
        .rx_data = NULL,
        .tx_data = init_data,
        .tx_length = LEN(init_data),
    };

    platform_i2c_transfer_blocking(&lcd_write);
}


void platform_lcd_clear_display(void) {

    I2C_M_SETUP_Type clear_packet = {
        .sl_addr7bit = LCD_ADDR,
        .rx_data = NULL,
    };

    uint8_t instructions[] = {
        Control_byte(0, 0),
        Display_ctl(0, 0, 0),
        Set_DDRAM(0x00)
    };
    uint8_t data[] = {
        Control_byte(0, 1),
        [1 ... 80] = 0x91 // 0x91 is a blank character
    };

    clear_packet.tx_data = instructions;
    clear_packet.tx_length = LEN(instructions);
    platform_i2c_transfer_blocking(&clear_packet);

    clear_packet.tx_data = data;
    clear_packet.tx_length = LEN(data);
    platform_i2c_transfer_blocking(&clear_packet);

    instructions[1] = Display_ctl(1, 0, 0);
    instructions[2] = Return_home;
    clear_packet.tx_data = instructions;
    clear_packet.tx_length = LEN(instructions);
    platform_i2c_transfer_blocking(&clear_packet);
}


void platform_lcd_send_bytes(uint8_t *data, uint32_t length) {
    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = LCD_ADDR,
        .rx_data = NULL,
    };

    packet.tx_data = data;
    packet.tx_length = length;

    platform_i2c_transfer_blocking(&packet);
}


void platform_lcd_write_bytes(uint8_t *bytes, uint8_t length, uint8_t ddram_addr) {
    length = min(DDRAM_SIZE, length);

    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = LCD_ADDR,
        .rx_data = NULL
    };

    uint8_t instructions[] = {
        Control_byte(0,0),
        Set_DDRAM(ddram_addr)
    };
    uint8_t data[length + 1];

    data[0] = Control_byte(0,1);

    memcpy(((void *)data) + 1, bytes, length);
    // Note that the DDRAM address wraps to the beginning of the line if it
    // increments past the end

    packet.tx_data = instructions;
    packet.tx_length = LEN(instructions);
    platform_i2c_transfer_blocking(&packet);

    packet.tx_data = data;
    packet.tx_length = length + 1;
    platform_i2c_transfer_blocking(&packet);
}


void platform_lcd_write_ascii(char *string, uint8_t ddram_addr) {
    // 0xe0 is the upside down ?
    static const uint8_t ascii_to_charset[128] = { [0 ... 127] = 0xe0,
        [' '] = 0x91, ['!'] = 0xa1, ['"'] = 0xa2, ['#'] = 0xa3, ['$'] = 0x82,
        ['%'] = 0xa5, ['&'] = 0xa6, ['\''] = 0xa7, ['('] = 0xa8, [')'] = 0xa9,
        ['*'] = 0xaa, ['+'] = 0xab, [','] = 0xac, ['-'] = 0xad, ['.'] = 0xae,
        ['/'] = 0xaf, ['0'] = 0xb0, ['1'] = 0xb1, ['2'] = 0xb2, ['3'] = 0xb3,
        ['4'] = 0xb4, ['5'] = 0xb5, ['6'] = 0xb6, ['7'] = 0xb7, ['8'] = 0xb8,
        ['9'] = 0xb9, [':'] = 0xba, [';'] = 0xbb, ['<'] = 0xbc, ['='] = 0xbd,
        ['>'] = 0xbe, ['?'] = 0xbf, ['@'] = 0x80, ['A'] = 0xc1, ['B'] = 0xc2,
        ['C'] = 0xc3, ['D'] = 0xc4, ['E'] = 0xc5, ['F'] = 0xc6, ['G'] = 0xc7,
        ['H'] = 0xc8, ['I'] = 0xc9, ['J'] = 0xca, ['K'] = 0xcb, ['L'] = 0xcc,
        ['M'] = 0xcd, ['N'] = 0xce, ['O'] = 0xcf, ['P'] = 0xd0, ['Q'] = 0xd1,
        ['R'] = 0xd2, ['S'] = 0xd3, ['T'] = 0xd4, ['U'] = 0xd5, ['V'] = 0xd6,
        ['W'] = 0xd7, ['X'] = 0xd8, ['Y'] = 0xd9, ['Z'] = 0xda, ['['] = 0x8a,
        [']'] = 0x54, ['_'] = 0x5a, ['a'] = 0xe1, ['b'] = 0xe2, ['c'] = 0xe3,
        ['d'] = 0xe4, ['e'] = 0xe5, ['f'] = 0xe6, ['g'] = 0xe7, ['h'] = 0xe8,
        ['i'] = 0xe9, ['j'] = 0xea, ['k'] = 0xeb, ['l'] = 0xec, ['m'] = 0xed,
        ['n'] = 0xee, ['o'] = 0xef, ['p'] = 0xf0, ['q'] = 0xf1, ['r'] = 0xf2,
        ['s'] = 0xf3, ['t'] = 0xf4, ['u'] = 0xf5, ['v'] = 0xf6, ['w'] = 0xf7,
        ['x'] = 0xf8, ['y'] = 0xf9, ['z'] = 0xfa
    };
    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = LCD_ADDR,
        .rx_data = NULL
    };
    uint8_t instructions[] = {
        Control_byte(0,0),
        Set_DDRAM(ddram_addr)
    };
    uint8_t data[DDRAM_SIZE+1] = {Control_byte(0,1)};
    uint8_t n=0;

    while (n < DDRAM_SIZE && string[n] != '\0') {
        data[n+1] = ascii_to_charset[string[n] < 128 ? string[n] : 0];
        n++;
    }

    packet.tx_data = instructions;
    packet.tx_length = LEN(instructions);
    platform_i2c_transfer_blocking(&packet);

    packet.tx_data = data;
    packet.tx_length = n+1;
    platform_i2c_transfer_blocking(&packet);
}

uint32_t platform_lcd_printf(uint8_t ddram_addr, char const *format, ...) {
    va_list args;
    char buf[16];

    va_start(args, format);
    int length = vsnprintf(buf, 16, format, args);
    va_end(args);

    if (length > 0) {
        platform_lcd_write_ascii(buf, ddram_addr);
    }
    return length;
}


static uint8_t min(uint8_t a, uint8_t b) {
    return (a < b) ? a : b;
}
