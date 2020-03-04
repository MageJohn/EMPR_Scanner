/*
This is part one of A1, this file combines all patters mentioned in A1
and lets the user choose between them.
*/

#include "platform.h"
#include "platform_lcd.h"
#include "platform_keypad.h"
#include "motor_patterns.h"
#include "time.h"

static void text_select_test(void);

void select_test(void) {
    bool key_pressed;
    uint8_t pressed_key;
    bool exit_condition = false;

    text_select_test();

    platform_keypad_use_lut(SCANCODE_CHAR_LUT);

    while(!exit_condition) {
        key_pressed = platform_keypad_poll_key_rl(&pressed_key, 500);

        if (key_pressed) {
            switch (pressed_key) {
            case 'A':
                motor_patterns_circle();
                break;
            case 'B':
                motor_patterns_square();
                break;
            case 'C':
                motor_patterns_zdemo();
                break;
            case '*':
                exit_condition = true;
                break;
            }
        }
    }
}


static void text_select_test(void) {
    platform_lcd_clear_display();

    platform_lcd_write_ascii("A->crcl / B->sqr", LCD_TOP_LINE);
    platform_lcd_write_ascii("C -> test Z axis", LCD_BOTTOM_LINE);
}
