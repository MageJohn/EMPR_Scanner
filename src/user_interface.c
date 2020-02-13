#include <stdio.h>
#include <string.h>

#include "ioboard.h"
#include "serial.h"
#include "platform.h"
#include "platform_keypad.h"
#include "platform_lcd.h"

bool exit_condition = false;
char data_to_screen[30];
uint8_t pressed_key;
char buffer[2];

int main(void) {

    serial_init();

    platform_init();
    platform_lcd_init();

    // Calibrate the head
	//platform_calibrate_head();
	//while(!platform_calibrated());

    platform_lcd_clear_display();
    strcpy(data_to_screen, "Test");
    platform_lcd_write_ascii(data_to_screen,0);

    //option_menu();

    return 0;

}

void option_menu() {

    text_option_menu();

    while(!exit_condition) {

        platform_keypad_poll_key(&pressed_key);

        if (pressed_key == 3) {

            // Go to motor patterns
            sprintf(buffer, "jj");
	        serial_write_b(buffer, 2);

        } else if (pressed_key == 2) {

            // Go to manual move
            sprintf(buffer, "aa");
	        serial_write_b(buffer, 2);

        }

        pressed_key = 30;

    }

}

void text_option_menu() {

    platform_lcd_clear_display();
    strcpy(data_to_screen, "A -> tests");
    platform_lcd_write_ascii(data_to_screen, 0);

    strcpy(data_to_screen, "B -> manual move");
    platform_lcd_write_ascii(data_to_screen, 64);

}