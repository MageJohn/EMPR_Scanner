#include "leds.h"
#include "platform.h"
#include "platform_lcd.h"


int main(void) {
    leds_init();
    platform_init();
    leds_mux_set_curr(HB_LED);

    platform_lcd_init();

    platform_lcd_clear_display();

    platform_lcd_write_ascii("hello", 0x00);
    platform_lcd_write_ascii("test", 0x40);

    return 0;
}
