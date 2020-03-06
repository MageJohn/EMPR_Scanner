#include <string.h>
#include "platform_keypad.h"
#include "platform_lcd.h"

#include "UI.h"


static void init_display(void);
static void draw_top_arrow(struct UI_elem *elem);
static void draw_bottom_arrow(struct UI_elem *elem);
static void show_menu(struct UI_elem *elem);

void UI_build_option_menu(struct UI_elem *last, struct UI_elem *this) {
    if (last != NULL) {
        last->next = this;
    }
    this->prev = last;
}

void UI_run(struct UI_elem *first) {
    uint8_t pressed_key;

    platform_lcd_clear_display();

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
    uint8_t out[2];
    out[1] = 0x20; // filled >
    out[0] = 0xc1; // A

    platform_lcd_write_bytes(out, 2, LCD_TOP_LINE);

    out[0] = 0xc2; // B

    platform_lcd_write_bytes(out, 2, LCD_BOTTOM_LINE);
}

void draw_top_arrow(struct UI_elem *elem) {
    uint8_t out[2];
    if (elem->prev != NULL) {
        // upparrow C
        out[0] = 0x12;
        out[1] = 0xc3;
    } else {
        out[0] = 0x91;
        out[1] = 0x91;
    }
    platform_lcd_write_bytes(out, 2, LCD_TOP_LINE + (0x10 - 2));
}

void draw_bottom_arrow(struct UI_elem *elem) {
    uint8_t out[2];
    if (elem->next != NULL) {
        // upparrow C
        out[0] = 0x30;
        out[1] = 0xc4;
    } else {
        out[0] = 0x91;
        out[1] = 0x91;
    }
    platform_lcd_write_bytes(out, 2, LCD_BOTTOM_LINE + (0x10 - 2));
}

void show_menu(struct UI_elem *elem) {
    platform_lcd_printf(LCD_TOP_LINE + 2, "%-12s", elem->text);
    draw_top_arrow(elem);
    if (elem->next != NULL) {
        platform_lcd_printf(LCD_BOTTOM_LINE + 2, "%-12s", elem->next->text);
        draw_bottom_arrow(elem->next);
    }
}
