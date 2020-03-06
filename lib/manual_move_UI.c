/*

This is part A3 and B1 combined into one file,
since they are almost identical, but differs only
by output to LCD screen.

*/

#include <stdio.h>

#include "platform.h"
#include "platform_keypad.h"
#include "platform_lcd.h"
#include "time.h"

#include "manual_move_UI.h"

struct Pos {
    int16_t x;
    int16_t y;
    int16_t z;
};


// File private functions
static void choosing_axis_info(void);
static void display_coordinates(void);
static void display_rgb_values(void);


void manual_ui(enum ManualUIDataDisplay mode, int16_t *out_coords) {
    bool exit_condition = false;
    uint8_t pressed_key;
    struct Pos coords;

    platform_keypad_use_lut(SCANCODE_CHAR_LUT);

    // Displays info on LCD about movement
    choosing_axis_info();

    while(!exit_condition) {
        // Gets and saves pressed key
        bool found_key = platform_keypad_poll_key(&pressed_key);

        platform_head_get_position(&coords.x, &coords.y, &coords.z);

        if (found_key) {
            switch (pressed_key) {
            case '2':
                coords.x = X_SOFT_LIMIT;
                break;
            case '5':
                coords.x = 0;
                break;
            case '6':
                coords.y = Y_SOFT_LIMIT;
                break;
            case '4':
                coords.y = 0;
                break;
            case 'A':
                coords.z = Z_SOFT_LIMIT;
                break;
            case 'B':
                coords.z = 0;
                break;
            case '*':
                exit_condition = true;
                break;
            case 'D':
                // Switch between showing axis position and rgb values
                mode = !mode;
                platform_lcd_clear_display();
                wait_ms(50);
                break;
            }
            wait_ms(100);
        } else {
            if (mode == UI_SHOW_RGB) {
                display_rgb_values();
            }
        }

        platform_head_set_coords(coords.x, coords.y, coords.z);

        if (mode == UI_SHOW_COORDS) {
            display_coordinates();
        }
    }

    if (out_coords != NULL) {
        out_coords[X] = coords.x;
        out_coords[Y] = coords.y;
        out_coords[Z] = coords.z;
    }
}

// Information on manual movement
static void choosing_axis_info(void) {
    platform_lcd_write_ascii("Manual move mode", LCD_TOP_LINE);
    platform_lcd_write_ascii("                ", LCD_BOTTOM_LINE);
    wait(1);
    platform_lcd_write_ascii("       2       A", LCD_TOP_LINE);
    platform_lcd_write_ascii("X/Y: 4 5 6  Z: B", LCD_BOTTOM_LINE);
    wait(2);
    platform_lcd_clear_display();
}

static void display_rgb_values(void) {
    union ColourData crgb;
    uint8_t rgb_display[3];

    platform_sensor_get_data(crgb.combined);

    // Convert from 16-bit raw data to 8-bit RGB representation
    rgb_display[0] = ((float)crgb.combined[0] / 1024.0) * 255;
    rgb_display[1] = ((float)crgb.combined[2] / 1024.0) * 255;
    rgb_display[2] = ((float)crgb.combined[3] / 1024.0) * 255;

    platform_lcd_printf(LCD_TOP_LINE,    "R: %3d", rgb_display[0]);

    platform_lcd_printf(LCD_BOTTOM_LINE, "G: %3d B: %3d",
                        rgb_display[1], rgb_display[2]);
}

static void display_coordinates(void) {
    struct Pos coords;
    platform_head_get_position(&coords.x, &coords.y, &coords.z);

    platform_lcd_printf(LCD_TOP_LINE,    "X: %3d Y: %3d", coords.x, coords.y);

    platform_lcd_printf(LCD_BOTTOM_LINE, "Z: %3d", coords.z);
}
