#include <stdio.h>
#include <string.h>

#include "ioboard.h"
#include "serial.h"
#include "platform.h"
#include "platform_keypad.h"
#include "platform_lcd.h"
#include "motor_patterns_UI.h"
#include "manual_move_UI.h"
#include "platform_edge_detection.h"
#include "time.h"

bool exit_condition_UI = false;
char data_to_screen[30];
uint8_t pressed_key;

void option_menu(void);
void text_option_menu(void);

int main(void) {

    serial_init();

    platform_init();
    platform_lcd_init();

    // Calibrate the head
    platform_calibrate_head();
    while(!platform_calibrated());

    // Set limits
    detect_edges();

    // Wait for reading the set edge limits
    wait(1);

    // Go into UI
    option_menu();

    return 0;

}

void option_menu(void) {

    text_option_menu();

    while(!exit_condition_UI) {

        platform_keypad_poll_key(&pressed_key);

        if (pressed_key == 3) {

            // Go to motor patterns UI
            // Calibrate the head
	        platform_calibrate_head();
	        while(!platform_calibrated());
            select_test();
            text_option_menu();

        } else if (pressed_key == 2) {

            // Go to manual move UI
            // Calibrate the head
	        platform_calibrate_head();
	        while(!platform_calibrated());
            manual_ui();
            text_option_menu();

        }

        pressed_key = 30;

    }

}

void text_option_menu(void) {

    platform_lcd_clear_display();
    strcpy(data_to_screen, "A -> tests");
    platform_lcd_write_ascii(data_to_screen, 0);

    strcpy(data_to_screen, "B -> manual move");
    platform_lcd_write_ascii(data_to_screen, 64);

}
