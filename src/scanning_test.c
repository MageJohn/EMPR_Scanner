#include "string.h"
#include "scanning.h"
#include "platform.h"
#include "serial.h"
#include "platform_lcd.h"

char num2hex(uint8_t num);

char num2hex(uint8_t num) {
    if(num >= 0x0a) {
        num += (0x41 - 0x0a);
    }
    else {
        num += 0x30;
    }
    return (char)num;
}

void main(void) {
    struct ScanningConfig cfg = {
        .z = 40,
        .start = {150, 120},
        .size = {500, 720},
        .res = {50, 72},
        .cal_freqs = {1, 1},
        .wait_for_sensor = false,
        .send_data = true,
        .show_lcd = false,
        .highest_vals = {10, 15, 20, 30}
    };

    platform_init();

    platform_calibrate_head();
    while(!platform_calibrated());

    serial_init();
    serial_wait_for_byte();
    scanning_setup(&cfg);
    scanning_raster(X, Y);
    char hex_vals[16];
    hex_vals[15] = '\0';
    int i;
    for (i = 0; i < 4; i++) {
        uint8_t *as_bytes = (uint8_t *)&cfg.highest_vals[i];
        hex_vals[4 * i] = num2hex(as_bytes[1] & 0x0f);
        hex_vals[4 * i + 1] = num2hex(as_bytes[1] & 0xf0);
        hex_vals[4 * i + 2] = num2hex(as_bytes[0] & 0x0f);
        hex_vals[4 * i + 3] = num2hex(as_bytes[0] & 0xf0);
    }
    platform_lcd_clear_display();
    platform_lcd_write_ascii(hex_vals, LCD_TOP_LINE);
    
}
