#include <stdio.h>
#include "platform.h"
#include "platform_lcd.h"
#include "serial.h"
#include "leds.h"

#include "scanning.h"

// File private functions
static void set_highest_crgb(union ColourData *cdata, int16_t x, int16_t y, int16_t z);
static void write_to_lcd(union ColourData *cdata);
static void calibrate(void);

// File globals
static struct ScanningConfig *cfg;

static struct ScanningState {
    int16_t step[2];
    int16_t stop[2];
} state;


/* ---------------------
 * Public functons
 * ---------------------*/

// Expects serial_init to have been called if cfg->send_data is set.
void scanning_setup(struct ScanningConfig *new_cfg) {
    cfg = new_cfg;

    if (cfg->show_lcd) {
        platform_lcd_init();
        platform_lcd_clear_display();
    }

    if (cfg->send_data) {
        serial_write_b((char *)cfg->res, sizeof(cfg->res));
    }

    state.step[X] = cfg->size[X]/cfg->res[X];
    state.step[Y] = cfg->size[Y]/cfg->res[Y];

    state.stop[X] = (cfg->start[X] + cfg->res[X] * state.step[X]);
    state.stop[Y] = (cfg->start[Y] + cfg->res[Y] * state.step[Y]);
}

void scanning_scan_axis(int16_t *coords, uint8_t axis) {
    union ColourData cdata;
    int16_t pos[3] = {coords[X], coords[Y], cfg->z};
    for (; pos[axis] != state.stop[axis]; pos[axis] += state.step[axis]) {
        platform_head_set_coords_and_wait(pos[X], pos[Y], pos[Z]);
        if (cfg->wait_for_sensor) {
            platform_sensor_wait_for_integration();
        }
        platform_sensor_get_data(cdata.combined);
        set_highest_crgb(&cdata, pos[0], pos[1], cfg->z);
        if (cfg->send_data) {
            while(!serial_nb_write_finished());
            serial_write_nb((char *)&cdata, sizeof(cdata));
        }
        if (cfg->show_lcd) {
            write_to_lcd(&cdata);
        }
    }
}

void scanning_raster(uint8_t axis1, uint8_t axis2) {
    int16_t coords[2] = {cfg->start[X], cfg->start[Y]};
    for (; coords[axis1] != state.stop[axis1]; coords[axis1] += state.step[axis1]) {
        scanning_scan_axis(coords, axis2);
        calibrate();
    }
}

void scanning_two_way_raster(uint8_t axis1, uint8_t axis2) {
    int16_t coords[2] = {cfg->start[X], cfg->start[Y]};
    for (; coords[axis1] != state.stop[axis1]; coords[axis1] += state.step[axis1]) {
        scanning_scan_axis(coords, axis2);
        calibrate();
    }
}


/*--------------------------
 * File private functions
 * -------------------------*/

static void write_to_lcd(union ColourData *cdata) {
    char buf[32];
    int8_t max_width = snprintf(buf, 32, "%hu", platform_sensor_get_integ_cycles() * 1024);

    snprintf(buf, 32, "R %.*hu G %.*hu",
             max_width, cdata->combined[1],
             max_width, cdata->combined[2]);
    platform_lcd_write_ascii(buf, LCD_TOP_LINE);

    snprintf(buf, 32, "B %.*hu",
             max_width, cdata->combined[3]);
    platform_lcd_write_ascii(buf, LCD_BOTTOM_LINE);

}

static void calibrate(void) {
    int16_t coords[3];
    platform_head_get_position(coords, coords + 1, coords + 2);
    if (cfg->cal_freqs[X] > 0 && (coords[X]/state.step[X]) % cfg->cal_freqs[X] == 0) {
        coords[X] = -10;
    }

    if (cfg->cal_freqs[Y] > 0 && (coords[Y]/state.step[Y]) % cfg->cal_freqs[Y] == 0) {
        coords[Y] = -10;
    }
    platform_head_set_coords_and_wait(coords[X], coords[Y], coords[Z]);
}

static void set_highest_crgb(union ColourData *cdata, int16_t x, int16_t y, int16_t z) {    
    int i;
    if (cdata->combined[0] >= cfg->highest_vals[0]) {
        cfg->highest_vals[0] = cdata->combined[0];
        cfg->location_highest[0][0] = x;
        cfg->location_highest[0][1] = y;
        cfg->location_highest[0][2] = z;
    }
    for(i = 1; i < 4; i++) {
        if (cdata->combined[i] >= cfg->highest_vals[i]
            && cdata->combined[(i % 3) + 1] < cdata->combined[i] * 0.8
            && cdata->combined[((i + 1) % 3) + 1] < cdata->combined[i] * 0.8
            ) {
            cfg->highest_vals[i] = cdata->combined[i];
            cfg->location_highest[i][0] = x;
            cfg->location_highest[i][1] = y;
            cfg->location_highest[i][2] = z;
        }
    }
}

