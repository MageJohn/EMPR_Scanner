#include "leds.h"
#include "platform.h"


int main(void) {
    led_setup();
    platform_init();
    led_mux_set_curr(HB_LED);

    platform_lcd_init();

    platform_lcd_clear_display();

    platform_lcd_write_ascii("hello", 0x00);
    platform_lcd_write_ascii("world", 0x40);

    return 0;
}