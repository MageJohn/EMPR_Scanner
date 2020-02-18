#include <string.h>
#include <stdio.h>
#include <math.h>

#include "platform_keypad.h"
#include "platform_lcd.h"
#include "serial.h"
#include "time.h"
#include "platform.h"


// Assumes previous platform init. with platform_init()
//                  LCD init. platform_lcd_init()

char data_to_screen[16];

bool cleared_once = false;
bool exit_condition;
bool key_down = false;
bool rgb_flag = false;

uint8_t pressed_key = 30;

uint8_t step_size = 5;

uint16_t x_axis = 0;
uint16_t y_axis = 0;
uint16_t z_axis = 0;

uint16_t x_display = 0;
uint16_t y_display = 0;
uint16_t z_display = 0;

uint64_t rgb_buffer[1];

uint8_t red_display;
uint8_t green_display;
uint8_t blue_display;

uint16_t* x_axis_pointer = &x_axis;
uint16_t* y_axis_pointer = &y_axis;
uint16_t* z_axis_pointer = &z_axis;

void move_axis_up(uint16_t *axis);
void move_axis_down(uint16_t *axis);
void manual_ui(void);
void choosing_axis_info(void);
void display_coordinates(void);
void get_rgb_values(void);
void display_rgb_values(void);


// Main loop, checks for input
void manual_ui(void) {

    // Displays info on LCD about movement
    choosing_axis_info();
    pressed_key = 30;
    exit_condition = false;
    cleared_once = false;

    wait_ms(300);

    while(!exit_condition) {

        // Gets and saves pressed key
        platform_keypad_poll_key(&pressed_key);

        if (pressed_key == 11) {

            key_down = true;
            move_axis_up(x_axis_pointer);

        } else if (pressed_key == 9) {

            key_down = true;
            move_axis_down(x_axis_pointer);

        } else if (pressed_key == 6) {

            key_down = true;
            move_axis_up(y_axis_pointer);

        } else if (pressed_key == 14) {

            key_down = true;
            move_axis_down(y_axis_pointer);

        } else if (pressed_key == 3) {

            key_down = true;
            move_axis_up(z_axis_pointer);

        } else if (pressed_key == 2) {

            key_down = true;
            move_axis_down(z_axis_pointer);

        } else if(pressed_key == 12) {
            
            // GO back to UI and exit manual move
            exit_condition = true;

        } else if(pressed_key == 0) {

            // Switch between showing axis position and rgb values
            if(!rgb_flag) {
                rgb_flag = true;
            } else if (rgb_flag) {
                rgb_flag = false;
            }

            platform_lcd_clear_display();
            wait_ms(200);
        
        } else {

            // Checks if no key is pressed and stops the movement
            key_down = false;
            platform_head_get_coords(&x_axis, &y_axis, &z_axis);
            platform_head_set_coords(x_axis, y_axis, z_axis);
            while(!platform_head_at_coords);

        }

        // Display moving information only once, otherwise clean it
        if ( (pressed_key != 30) && !cleared_once) {
            platform_lcd_clear_display();
            cleared_once = true;
        } else if (cleared_once) {
            
            // Display coordinates/rgb value to lcd constantly

            if (!rgb_flag) {
                display_coordinates();
            } else if (rgb_flag) {
                display_rgb_values();
            }

        }

        pressed_key = 30;

    }

}

// Information on manual movement
void choosing_axis_info() {

    platform_lcd_clear_display();

    strcpy(data_to_screen, "To move XYZ axis");
    platform_lcd_write_ascii(data_to_screen, 0);

    strcpy(data_to_screen, "manually..");
    platform_lcd_write_ascii(data_to_screen, 64);

    wait(1);

    platform_lcd_clear_display();

    strcpy(data_to_screen, "Press: 2/8 -> X");
    platform_lcd_write_ascii(data_to_screen, 0);

    strcpy(data_to_screen, "4/6->Y or A/B->Z");
    platform_lcd_write_ascii(data_to_screen, 64);

    pressed_key = 30;

}


void display_rgb_values() {

    get_rgb_values();

    sprintf(data_to_screen, "R: %3d", red_display);
    platform_lcd_write_ascii(data_to_screen, 0);

    sprintf(data_to_screen, "G: %3d B: %3d", green_display, blue_display);
    platform_lcd_write_ascii(data_to_screen, 64);

}


void get_rgb_values() {

    platform_sensor_get_data(&rgb_buffer);

    uint16_t *p = (uint16_t *)&rgb_buffer;

    // Convert from 16-bit raw data to 8-bit RGB representation
    red_display = ((float)p[1]/65536)*255;
    green_display = ((float)p[2]/65536)*255;
    blue_display = ((float)p[3]/65536)*255;

}


void display_coordinates() {

    platform_head_get_coords(&x_display, &y_display, &z_display);

    sprintf(data_to_screen, "X: %4d Y: %4d", x_display, y_display);
    platform_lcd_write_ascii(data_to_screen, 0);

    sprintf(data_to_screen, "Z: %4d", z_display);
    platform_lcd_write_ascii(data_to_screen, 64);

}


void move_axis_up(uint16_t *axis) {

    platform_head_get_coords(x_axis_pointer, y_axis_pointer, z_axis_pointer);

    // Check for upper bounds
    if( (axis == x_axis_pointer && x_axis < X_SOFT_LIMIT)
        || (axis == y_axis_pointer && y_axis < Y_SOFT_LIMIT) 
        || (axis == z_axis_pointer && z_axis < Z_SOFT_LIMIT)) {

        // Check for controlled axis
        if(axis == x_axis_pointer) {
            x_axis = X_SOFT_LIMIT;
        } else if (axis == y_axis_pointer) {
            y_axis = Y_SOFT_LIMIT;
        } else if (axis == z_axis_pointer) {
            z_axis = Z_SOFT_LIMIT;
        }

	    platform_head_set_coords(x_axis, y_axis, z_axis);

    }

}


void move_axis_down(uint16_t *axis) {

    platform_head_get_coords(x_axis_pointer, y_axis_pointer, z_axis_pointer);

    // Check for lower bounds
    if( (*axis)>0 ) {

        // Checks for controlled axis
        if(axis == x_axis_pointer) {
            x_axis = 0;
        } else if (axis == y_axis_pointer) {
            y_axis = 0;
        } else if (axis == z_axis_pointer) {
            z_axis = 0;
        }

	    platform_head_set_coords(x_axis, y_axis, z_axis);
        
    }

}