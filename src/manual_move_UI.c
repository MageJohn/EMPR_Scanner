#include <string.h>
#include <stdio.h>

#include "platform_keypad.h"
#include "platform_lcd.h"
#include "serial.h"
#include "time.h"
#include "platform.h"


// Assumes previous platform init. with platform_init()
//                  LCD init. platform_lcd_init()

char data_to_screen[16];
char buffer[32];

bool cleared_once = false;
bool exit_condition = false;
bool key_down = false;

uint8_t pressed_key = 30;

uint8_t step_size = 5;

uint16_t x_axis = 0;
uint16_t y_axis = 0;
uint16_t z_axis = 0;

uint16_t x_display = 0;
uint16_t y_display = 0;
uint16_t z_display = 0;

uint16_t* x_axis_pointer = &x_axis;
uint16_t* y_axis_pointer = &y_axis;
uint16_t* z_axis_pointer = &z_axis;

void move_axis_up(uint16_t *axis);
void move_axis_down(uint16_t *axis);
void manual_ui(void);
void choosing_axis_info();

// TEST
int main(void) {

    serial_init();

    platform_init();
    platform_lcd_init();

    platform_calibrate_head();
    while(!platform_calibrated());

    manual_ui();

    return 1;

}

void manual_ui(void) {

    choosing_axis_info();

    while(!exit_condition) {

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

        } else {

            key_down = false;
            platform_head_get_coords(&x_axis, &y_axis, &z_axis);
            platform_head_set_coords(x_axis, y_axis, z_axis);
            while(!platform_head_at_coords);

        }

        if ( (pressed_key != 30) && !cleared_once) {
            platform_lcd_clear_display();
            cleared_once = true;
        } else if (cleared_once) {
            platform_head_get_coords(&x_display, &y_display, &z_display);
            display_coordinates();
        }

        pressed_key = 30;

    }

}


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

}


void display_coordinates() {

    sprintf(data_to_screen, "X: %4d Y: %4d", x_axis, y_axis);
    platform_lcd_write_ascii(data_to_screen, 0);

    sprintf(data_to_screen, "Z: %4d", z_axis);
    platform_lcd_write_ascii(data_to_screen, 64);

}


void move_axis_up(uint16_t *axis) {

    //sprintf(buffer, "%d %d \n\r", axis, x_axis_pointer);
    //serial_write_b(buffer, 32);

    platform_head_get_coords(x_axis_pointer, y_axis_pointer, z_axis_pointer);

    if( (axis == x_axis_pointer && x_axis < X_SOFT_LIMIT)
        || (axis == y_axis_pointer && y_axis < Y_SOFT_LIMIT) 
        || (axis == z_axis_pointer && z_axis < Z_SOFT_LIMIT)) {

	    //(*axis) += step_size;

        if(axis == x_axis_pointer) {
            x_axis = X_SOFT_LIMIT;
        } else if (axis == y_axis_pointer) {
            y_axis = Y_SOFT_LIMIT;
        } else if (axis == z_axis_pointer) {
            z_axis = Z_SOFT_LIMIT;
        }

	    platform_head_set_coords(x_axis, y_axis, z_axis);

        while(key_down) {
            platform_head_get_coords(&x_display, &y_display, &z_display);
            display_coordinates();
        }

        sprintf(buffer, "%d %d %d \n\r ", x_axis, y_axis, z_axis);
        serial_write_b(buffer, 32);

    }

}


void move_axis_down(uint16_t *axis) {

    platform_head_get_coords(x_axis_pointer, y_axis_pointer, z_axis_pointer);

    if( (*axis)>0 ) {

	    //(*axis) -= step_size;

        if(axis == x_axis_pointer) {
            x_axis = 0;
        } else if (axis == y_axis_pointer) {
            y_axis = 0;
        } else if (axis == z_axis_pointer) {
            z_axis = 0;
        }

	    platform_head_set_coords(x_axis, y_axis, z_axis);
        while(key_down) {
            platform_head_get_coords(&x_display, &y_display, &z_display);
            display_coordinates();
        }

        sprintf(buffer, "%d %d %d \n\r", x_axis, y_axis, z_axis);
        serial_write_b(buffer, 32);

    }

}