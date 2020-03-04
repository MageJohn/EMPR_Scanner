/*

This file combines parts A1, A2, A3 and B1 into a single user interface
file, from which the previously mentioned parts can be accessed
and demonstrated.

*/

#include "serial.h"
#include "platform.h"
#include "platform_keypad.h"
#include "platform_lcd.h"
#include "motor_patterns_UI.h"
#include "manual_move_UI.h"
#include "platform_edge_detection.h"
#include "time.h"

uint8_t pressed_key;

void option_menu(void);
void text_option_menu(void);

void main(void) {
    serial_init();
    platform_init();
    platform_lcd_init();
    platform_lcd_clear_display();

    platform_calibrate_head();
    while(!platform_calibrated());

    // Go into UI
    option_menu();
}

void option_menu(void) {
    platform_keypad_use_lut(SCANCODE_CHAR_LUT);
    text_option_menu();

    while(true) {
        bool key_pressed = platform_keypad_poll_key_rl(&pressed_key, 500);

        if (key_pressed) {
            switch (pressed_key) {
            case 'A':
                // Go to motor patterns UI
                // Calibrate the head
                select_test();
                break;
            case 'B':
                // Start edge detection
                detect_edges();
                break;
            case 'C':
                // Go to manual move UI
                // Calibrate the head
                manual_ui(UI_SHOW_COORDS);
                break;
            }
        }
    }
}

void text_option_menu(void) {
    platform_lcd_write_ascii("A> patt", LCD_TOP_LINE);
    platform_lcd_write_ascii("B> edge", LCD_TOP_LINE + 8);
    platform_lcd_write_ascii("C> move/scan", LCD_BOTTOM_LINE);

    // put arrow chars on the display
    /* uint8_t rarrow = 0x20; */
    /* platform_lcd_write_bytes(&rarrow, 1, LCD_TOP_LINE + 1); */
    /* platform_lcd_write_bytes(&rarrow, 1, LCD_TOP_LINE + 8 + 1); */
    /* platform_lcd_write_bytes(&rarrow, 1, LCD_BOTTOM_LINE + 1); */
}
