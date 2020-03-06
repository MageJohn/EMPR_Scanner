#include "platform_keypad.h"
#include "platform_lcd.h"

#include "UI.h"


static void init_display(void);
static void show_menu(struct UI_elem *elem);

void UI_build_sequence(struct UI_elem *last, struct UI_elem *this) {
    if (last != NULL) {
        last->next = this;
    }
    this->prev = last;
}

void UI_run(struct UI_elem *first) {
    uint8_t pressed_key;

    platform_keypad_use_lut(SCANCODE_CHAR_LUT);
    init_display();
    show_menu(first);

    while(true) {
        bool key_pressed = platform_keypad_poll_key_rl(&pressed_key, 500);

        if (key_pressed) {
            switch (pressed_key) {
            case 'A':
                first->callback();
                init_display();
                break;
            case 'B':
                first->next->callback();
                init_display();
                break;
            case 'C':
                if (first->prev != NULL) {
                    first = first->prev;
                }
                break;
            case 'D':
                if (first->next != NULL && first->next->next != NULL) {
                    first = first->next;
                }
                break;
            case '*':
                return;
            }

            show_menu(first);
        }
    }
}

void init_display(void) {
    platform_lcd_write_ascii("A>", LCD_TOP_LINE);
    platform_lcd_write_ascii("B>", LCD_BOTTOM_LINE);

    // upparrow C
    platform_lcd_write_bytes((uint8_t[]){0x12, 0xc3}, 2, LCD_TOP_LINE + (0x10 - 2));
    // downarrow D
    platform_lcd_write_bytes((uint8_t[]){0x30, 0xc4}, 2, LCD_BOTTOM_LINE + (0x10 - 2));
}

void show_menu(struct UI_elem *elem) {
    platform_lcd_write_ascii(elem->text, LCD_TOP_LINE + 2);
    if (elem->next != NULL) {
        platform_lcd_write_ascii(elem->next->text, LCD_BOTTOM_LINE + 2);
    }
}
