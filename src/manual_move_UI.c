#include "ioboard.h"
#include "serial.h"
#include <string.h>
#include "manual_platform_move.h"

// Assumes previous I2c init. with ioboard_i2c_init()
//                  LCD init. ioboard_lcd_init()

char data_to_screen[30] = "To move XYZ axis";
bool exit_condition = false;
uint8_t pressed_key;

bool x_flag = false;
bool y_flag = false;
bool z_flag = false;

// TEST
int main(void) {

    serial_init();

    ioboard_i2c_init();
    ioboard_lcd_init();
    manual_ui();

}

void manual_ui(void) {

    ioboard_lcd_clear_display();

    ioboard_lcd_write_ascii(data_to_screen, 0);

    strcpy(data_to_screen, "manually..");
    ioboard_lcd_write_ascii(data_to_screen, 64);

    wait(2);

    ioboard_lcd_clear_display();

    strcpy(data_to_screen, "..press: A -> X");
    ioboard_lcd_write_ascii(data_to_screen, 0);

    strcpy(data_to_screen, "B -> Y or C -> Z");
    ioboard_lcd_write_ascii(data_to_screen, 64);

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

        } else if (x_flag == true) {

            move_along_axis(x_flag, x_axis);

        } else if (y_flag == true) {

            move_along_axis(y_flag, y_axis);

        } else if (z_flag == true) {

            move_along_axis(z_flag, z_axis);

        }

        pressed_key = 30;

    }

}


void ui_moving_info(char axis) {

        ioboard_lcd_clear_display();
        strcpy(data_to_screen, "Press A or B to");
        ioboard_lcd_write_ascii(data_to_screen, 0);

        sprintf(data_to_screen, "move %c axis motor", axis);
        ioboard_lcd_write_ascii(data_to_screen, 64);

}


void move_along_axis(bool flag, uint16_t axis) {

    while(flag == true) {

        if (pressed_key == 3) {
            move_axis_up(axis);
        } else if (pressed_key == 2) {
            move_axis_down(axis);
        } else if (pressed_key == 12) {
            flag = false;
        }

        pressed_key = 30;

    }

}