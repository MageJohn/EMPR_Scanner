/*

This is part one of A1, this file combines all patters mentioned in A1
and lets the user choose between them.

*/

#include <stdio.h>
#include <string.h>

#include "ioboard.h"
#include "serial.h"
#include "platform.h"
#include "platform_lcd.h"
#include "platform_keypad.h"
#include "motor_patterns.h"
#include "time.h"

bool exit_condition_motor;
uint8_t pressed_key;
char data_to_screen[32];


void select_test(void);
void text_select_test(void);

void select_test(void) {

    text_select_test();
    pressed_key = 30;
    exit_condition_motor = false;

    wait_ms(300);

    while(!exit_condition_motor) {

        platform_keypad_poll_key(&pressed_key);

        if(pressed_key == 3) {

            while(!draw_circle());

        } else if (pressed_key == 2) {

            while(!draw_square());

        } else if (pressed_key == 1) {

            while(!test_vertical_axis());

        } else if (pressed_key == 12) {

            exit_condition_motor = true;

        }

        pressed_key = 30;

    }

}


void text_select_test(void) {

    platform_lcd_clear_display();
    strcpy(data_to_screen, "A->crcl / B->sqr");
    platform_lcd_write_ascii(data_to_screen, 0);

    strcpy(data_to_screen, "C -> test Z axis");
    platform_lcd_write_ascii(data_to_screen, 64);

    pressed_key = 30;

}
