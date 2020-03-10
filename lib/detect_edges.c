/*
  This is A2, detects edges and sets limits.
*/

#include <stdlib.h>

#include "platform_lcd.h"
#include "serial.h"
#include "platform.h"

#define THRESHOLD 10
#define SEARCH_STEP 5
#define Z_HEIGHT 500

static void find_edge(int16_t *coords, uint8_t axis, int16_t step, int16_t stop);

int16_t detected_edges[2][2] = {{0, 0}, {X_SOFT_LIMIT - 10, Y_SOFT_LIMIT - 10}};


void detect_edges(void) {
    platform_calibrate_head();

    platform_lcd_clear_display();
    platform_lcd_write_ascii("Bottom corner", LCD_TOP_LINE);

    while(!platform_calibrated());

    find_edge(detected_edges[0], X, SEARCH_STEP, X_SOFT_LIMIT / 10);
    find_edge(detected_edges[0], Y, SEARCH_STEP, Y_SOFT_LIMIT / 10);

    platform_calibrate_head();

    platform_lcd_printf(LCD_TOP_LINE, "%.3u %.3u           ",
                        detected_edges[0][X], detected_edges[0][Y]);
    platform_lcd_write_ascii("Top corner", LCD_BOTTOM_LINE);

    while(!platform_calibrated());

    find_edge(detected_edges[1], X, SEARCH_STEP, X_SOFT_LIMIT);
    find_edge(detected_edges[1], Y, SEARCH_STEP, Y_SOFT_LIMIT);

    platform_lcd_printf(LCD_BOTTOM_LINE, "%.3u %.3u        ",
                        detected_edges[1][X], detected_edges[1][Y]);
}


static void find_edge(int16_t *coords, uint8_t axis, int16_t step, int16_t stop) {
    int16_t pos[2] = {X_SOFT_LIMIT / 2, Y_SOFT_LIMIT / 2};
    uint16_t crgb[4];
    int16_t this_c;
    int16_t last_c;

    pos[axis] = coords[axis];
    platform_head_set_coords_and_wait(pos[0], pos[1], Z_HEIGHT);

    platform_sensor_get_data(crgb);
    last_c = crgb[0];

    for (; pos[axis] < stop; pos[axis] += step) {
        platform_head_set_coords_and_wait(pos[0], pos[1], Z_HEIGHT);

        platform_sensor_get_data(crgb);
        this_c = crgb[0];
        if (abs(last_c - this_c) > THRESHOLD) {
            break;
        }

        last_c = this_c;
    }

    coords[axis] = pos[axis];
}
