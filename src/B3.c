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
        .z = 100,
        .start = {300, 400},
        .size = {300, 400},
        .res = {10, 10},
        .cal_freqs = {0, 1},
        .wait_for_sensor = false,
        .send_data = true,
        .show_lcd = false,
        .highest_vals = {0, 0, 0, 0},
        .location_highest = {{0, 0, 0},{0, 0, 0},{0, 0, 0},{0, 0, 0}}
    };

    platform_calibrate_head();
    while(!platform_calibrated());

    scanning_setup(&cfg);
    leds_mux_set_curr(10);

    scanning_raster(X, Y);

    platform_lcd_printf(LCD_TOP_LINE, "%x", cfg.highest_vals[0]);

    int i;
    for(i = 0; i < 4; i++) {
        platform_head_set_coords(cfg.location_highest[i][0], cfg.location_highest[i][1], cfg.location_highest[i][2]);
        while(!platform_head_at_coords());
        //platform_lcd_write_ascii("Highest colour location", LCD_TOP_LINE);
        wait(5);
    }

}

