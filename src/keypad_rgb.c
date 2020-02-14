#include <stdio.h>
#include "lpc17xx_i2c.h"
#include "lpc17xx_systick.h"
#include "ioboard.h"
#include "serial.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "mbed.h"
#include "leds.h"
#include "platform.h"

uint8_t val = 0;
uint8_t updown = 0;
int xpos = 0;
char buffer[16] = "a";
uint16_t rgb = 0;
uint16_t x = 0;
uint16_t y = 0;
uint16_t z = 0;


void main(void){
	//Initialise
	platform_init();
	sensor_init();
	platform_lcd_init();
	platform_lcd_clear_display();
	led_setup();
	platform_calibrate_head();
	while (!platform_calibrated());
	platform_head_set_coords(0,0,0);
	
	
	//Write Menu Options To Screen
	platform_lcd_write_ascii("Mode:", 0x00);
	platform_lcd_write_ascii("A B or C", 0x40);

	while(1){
		//platform_i2c_read(0x21, &val, 8);
		//sprintf(buffer, "%d", val);
		//ioboard_keypad_rl_get_key(&val, 100);
		//platform_lcd_write_ascii(buffer, 0x40);

		//if (ioboard_keypad_poll()) {
			//move();
		//}

		val = 0;
	}
}


void move(void) {
	switch (val) {
		case 3: //A
			platform_lcd_clear_display();
			platform_lcd_write_ascii("X Axis Move", 0x00);
			platform_lcd_write_ascii("Press 1/4 or 0", 0x40);
			xaxis();

			break;
		case 2: //B
			platform_lcd_clear_display();
			platform_lcd_write_ascii("Y Axis Move", 0x00);
			platform_lcd_write_ascii("Press 1/4 or 0", 0x40);
			yaxis();
			break;
		case 1: //c
			platform_lcd_clear_display();
			platform_lcd_write_ascii("Z Axis Move", 0x00);
			platform_lcd_write_ascii("Press 1/4 or 0", 0x40);
			zaxis();
			break;
		default:
			return;
	}

}

void xaxis(void){
	while(1){
		ioboard_keypad_get_key(&updown);
		if (updown == 15) {
			// MOVE X AXIS FOWARD
			platform_head_set_coords(x, y, z);
			platform_sensor_get_data(&rgb);
			sprintf(buffer, "%d", rgb);
			platform_lcd_write_ascii("RGB Vals: 0=back", 0x00);
			platform_lcd_write_ascii(buffer, 0x40);
			x++;	
		} else if (updown == 14) {
			// MOVE X AXIS BACK
			platform_head_set_coords(x, y, z);
			platform_sensor_get_data(&rgb);
			sprintf(buffer, "%d", rgb);
			platform_lcd_write_ascii("RGB Vals: 0=back", 0x00);
			platform_lcd_write_ascii(buffer, 0x40);
			x--;
		} else if (updown == 8) {
			platform_lcd_clear_display();
			platform_lcd_write_ascii("Mode:", 0x00);
			platform_lcd_write_ascii("A B or C", 0x40);
			led_clear();
			return;
		}
		updown = 0;
		//DISPLAY RGB VALUES TO DISPLAY
	
	}
}

void yaxis(void){
	while(1){
		ioboard_keypad_get_key(&updown);
		if (updown == 15) {
			// MOVE Y AXIS FOWARD

			platform_sensor_get_data(&rgb);
			sprintf(buffer, "%d", rgb);
			platform_lcd_write_ascii("RGB Vals: 0=back", 0x00);
			platform_lcd_write_ascii(buffer, 0x40);			
		} else if (updown == 14) {
			// MOVE Y AXIS BACK
			platform_sensor_get_data(&rgb);
			sprintf(buffer, "%d", rgb);
			platform_lcd_write_ascii("RGB Vals: 0=back", 0x00);
			platform_lcd_write_ascii(buffer, 0x40);
		} else if (updown == 8) {
			platform_lcd_clear_display();
			platform_lcd_write_ascii("Mode:", 0x00);
			platform_lcd_write_ascii("A B or C", 0x40);
			led_clear();
			return;
		}
		updown = 0;
		//DISPLAY RGB VALUES TO DISPLAY
	
	}
}


void zaxis(void){
	while(1){
		ioboard_keypad_get_key(&updown);
		if (updown == 15) {
			// MOVE Z AXIS FOWARD
			//GET VALUES FROM RGB SENSOR
			platform_sensor_get_data(&rgb);
			sprintf(buffer, "%d", rgb);
			platform_lcd_write_ascii("RGB Vals: 0=back", 0x00);
			platform_lcd_write_ascii("R:100G:100B:100", 0x40);			
		} else if (updown == 14) {
			// MOVE Z AXIS BACK
			platform_sensor_get_data(&rgb);
			sprintf(buffer, "%d", rgb);
			platform_lcd_write_ascii("RGB Vals: 0=back", 0x00);
			platform_lcd_write_ascii("R:010G:100B:100", 0x40);
		} else if (updown == 8) {
			platform_lcd_clear_display();
			platform_lcd_write_ascii("Mode:", 0x00);
			platform_lcd_write_ascii("A B or C", 0x40);
			led_clear();
			return;
		}
		updown = 0;
		
	
	}
}



