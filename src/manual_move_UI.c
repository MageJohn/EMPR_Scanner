#include <string.h>
#include <stdio.h>

#include "ioboard.h"
#include "serial.h"
#include "platform.h"


// Assumes previous I2c init. with ioboard_i2c_init()
//                  LCD init. ioboard_lcd_init()

char data_to_screen[30];
bool exit_condition = false;
uint8_t pressed_key;
char buffer[2];

bool x_flag = false;
bool y_flag = false;
bool z_flag = false;

uint16_t x_axis = 0;
uint16_t y_axis = 0;
uint16_t z_axis = 100;

void move_axis_up(uint16_t axis);
void move_axis_down(uint16_t axis);
void manual_ui(void);
void ui_moving_info(char axis);
void move_along_axis(bool flag, uint16_t axis);
void choosing_axis_info();

// TEST
int main(void) {

    serial_init();

    ioboard_i2c_init();
    ioboard_lcd_init();

    //TODO: this
    //platform_init();
    //platform_calibrate_head();
    //while(!platform_calibrated())

    manual_ui();

}

void manual_ui(void) {

    choosing_axis_info();

    while(!exit_condition) {

        ioboard_keypad_get_key(&pressed_key);

        if (pressed_key == 3) {

            ui_moving_info('X');
            x_flag = true;
            
        } else if (pressed_key == 2) {

            ui_moving_info('Y');
            y_flag = true;

        } else if (pressed_key == 1) {

            ui_moving_info('Z');
            z_flag = true;

        }
        
        
        if (x_flag == true) {

            move_along_axis(x_flag, x_axis);
            x_flag = false;

        } else if (y_flag == true) {

            move_along_axis(y_flag, y_axis);
            y_flag = false;

        } else if (z_flag == true) {

            move_along_axis(z_flag, z_axis);
            z_flag = false;

        }

        pressed_key = 30;

    }

}


void ui_moving_info(char axis) {

        ioboard_lcd_clear_display();
        strcpy(data_to_screen, "Press A or B to");
        ioboard_lcd_write_ascii(data_to_screen, 0);

        sprintf(data_to_screen, "move %c axis", axis);
        ioboard_lcd_write_ascii(data_to_screen, 64);

}


void move_along_axis(bool flag, uint16_t axis) {

    bool newflag = flag;

    while (newflag == true) {

        ioboard_keypad_get_key(&pressed_key);

        if (pressed_key == 3) {

            // TODO: this
            //move_axis_up(axis);

        } else if (pressed_key == 2) {

            // TODO: this
            //move_axis_down(axis);

        } else if (pressed_key == 12) {
            flag = false;
            newflag = false;
            choosing_axis_info();
        }

        pressed_key = 30;

    }

}

void choosing_axis_info() {

    ioboard_lcd_clear_display();

    strcpy(data_to_screen, "To move XYZ axis");
    ioboard_lcd_write_ascii(data_to_screen, 0);

    strcpy(data_to_screen, "manually..");
    ioboard_lcd_write_ascii(data_to_screen, 64);

    wait(1);

    ioboard_lcd_clear_display();

    strcpy(data_to_screen, "..press: A -> X");
    ioboard_lcd_write_ascii(data_to_screen, 0);

    strcpy(data_to_screen, "B -> Y or C -> Z");
    ioboard_lcd_write_ascii(data_to_screen, 64);

}


void move_axis_up(uint16_t axis) {

	axis += 50;
	platform_head_set_coords(x_axis, y_axis, z_axis);

}


void move_axis_down(uint16_t axis) {

	axis -= 50;
	platform_head_set_coords(x_axis, y_axis, z_axis);

}