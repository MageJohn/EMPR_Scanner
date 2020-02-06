#include "ioboard.h"
#include "serial.h"

char data[1] = {0b11001000};

int main(void) {

	ioboard_i2c_init();
	ioboard_lcd_init();
	ioboard_lcd_clear_display();

	ioboard_lcd_write_ascii(data, 5);

}
