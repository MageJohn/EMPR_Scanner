#include "platform.h"
#include "platform_lcd.h"
#include "time.h"

void main(void) {
    platform_init();

    platform_lcd_printf(LCD_TOP_LINE, "Hello, %s!", "Micah");
}
