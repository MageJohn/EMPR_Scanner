#include <stdio.h>

#include "ioboard.h"
#include "serial.h"
#include "platform.h"

bool exit_condition = false;
uint8_t pressed_key;

int main(void) {

    serial_init();
    platform_init();
    platform_lcd_init();

    while(!draw_square());

    sprintf(buffer, "ok");
	serial_write_b(buffer, 2);
}


void select_test(void) {

    text_select_test();

    while(!exit_condition) {

        ioboard_keypad_get_key(&pressed_key);

        if(pressed_key == 3) {

            while(!draw_circle());

        } else if (pressed_key == 2) {

            while(!draw_square());

        } else if (pressed_key == 1) {

            while(!test_vertical_axis());

        } 

    }

}


void text_select_test(void) {

    platform_lcd_clear_display();
    strcpy(data_to_screen, "A->circle, B->square");
    platform_lcd_write_ascii(data_to_screen, 0);

    strcpy(data_to_screen, "C -> test Z axis");
    platform_lcd_write_ascii(data_to_screen, 64);

}