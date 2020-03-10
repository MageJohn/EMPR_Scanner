#include "scanning.h"
#include "platform.h"
#include "time.h"
#include <stdlib.h>
#include "serial.h"
#include "leds.h"
#include "platform_lcd.h"

void main(void) {
    platform_init();
    serial_init();
    platform_lcd_init();
    platform_lcd_clear_display();

    struct ScanningConfig cfg = {
        .z = 200,
        .start = {100, 600},
        .size = {600, 1},
        .res = {120, 1},
        .cal_freqs = {0, 0},
        .wait_for_sensor = false,
        .send_data = false,
        .show_lcd = false,
        .highest_vals = {0, 0, 0, 0},
        .location_highest = {{0, 0, 0},{0, 0, 0},{0, 0, 0},{0, 0, 0}}
    };

    platform_calibrate_head();
    while(!platform_calibrated());

    scanning_setup(&cfg);

    scanning_raster(X, Y);


    int i;
    for(i = 1; i < 4; i++) {
        platform_head_set_coords(cfg.location_highest[i][0], cfg.location_highest[i][1], cfg.location_highest[i][2]);
        platform_lcd_printf(LCD_TOP_LINE, "%x", cfg.highest_vals[i]);
        while(!platform_head_at_coords());
        wait(5);
    }

}

