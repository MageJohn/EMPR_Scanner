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

char buffer[8] = "H";
char buffer2[8] = "H";
uint8_t val = 0;
char a[4][15] = {"A - X Axis","B - Y Axis","C - Z Axis", "9 - Enter Value"};
int count = 0;
uint8_t updown = 0;
uint8_t postemp = 0;
int xpos = 0;

void move(void);
void xaxis(void);
void yaxis(void);
void zaxis(void);
void manualmove(void);
void main(void) {
	ioboard_i2c_init();
	ioboard_lcd_init();
	ioboard_lcd_clear_display();
	led_setup();
	
	platform_init();
	
	
	
	ioboard_lcd_write_ascii("Mode */# change", 0x00);
	ioboard_lcd_write_ascii(a[count], 0x40);
	while(1){
		ioboard_keypad_rl_get_key(&val, 100);
		//ioboard_keypad_get_key(&val);
		sprintf(buffer, "%d", val);
		//ioboard_keypad_rl_get_key(*pressed, 1);
		//sprintf(buffer2, "%s", platform_head_at_coords());
		//serial_write_b(buffer2, 8);
		
		serial_write_b(&buffer,5); //rmove after test

		//ioboard_lcd_write_ascii(&buffer, 0x00);
		if (ioboard_keypad_poll()) {
			move();
		}
		val = 0;
		
		
	}

}



void move(void) {
	switch (val) {
		case 3: //A
			ioboard_lcd_clear_display();
			ioboard_lcd_write_ascii("X Axis Move", 0x00);
			ioboard_lcd_write_ascii("Press 1/4 or 0", 0x40);
			xaxis();
			break;
		case 2: //B
			ioboard_lcd_clear_display();
			ioboard_lcd_write_ascii("Y Axis Move", 0x00);
			ioboard_lcd_write_ascii("Press 1/4 or 0", 0x40);
			yaxis();
			break;
		case 7: //3
			led_clear();
			led_disp_num(3);
			break;
		case 6: //6
			led_clear();
			led_disp_num(4);
			break;
		case 1: //c
			ioboard_lcd_clear_display();
			ioboard_lcd_write_ascii("Z Axis Move", 0x00);
			ioboard_lcd_write_ascii("Press 1/4 or 0", 0x40);
			zaxis();
			break;
		case 5: //9
			ioboard_lcd_clear_display();
			ioboard_lcd_write_ascii("Manual Move", 0x00);
			ioboard_lcd_write_ascii("Enter x Pos:", 0x40);
			manualmove();
			break;
		case 4: //#
			led_clear();
			led_disp_num(7);
			count++;
			if (count == 3){
				count = 0;
			}
			//sprintf(buffer2, "%d", count);
			//serial_write_b(buffer2,5);
	
			ioboard_lcd_write_ascii(&a[count], 0x40);
			break;
			
		case 12: //*
			led_clear();
			led_disp_num(8);
			count--;
			if (count == -1){
				count = 2;
			}
			//sprintf(buffer2, "%d", count);
			//serial_write_b(buffer2,5);
			ioboard_lcd_write_ascii(&a[count], 0x40);
			break;
		default:
			return;
	}

}

void xaxis(void){
	//DISPLAY Position as well
	//MOVE axis X Direction
	//platform_init();
	//struct LedSource *led = led_mux_register_source(10);
	//led_mux_set_curr(10);
	//platform_calibrate_head();
   // while (!platform_calibrated());
    //platform_head_set_coords(X_SOFT_LIMIT - 1, Y_SOFT_LIMIT -1, 7000);
	//platform_head_at_coords();
	//uint16_t x = X_SOFT_LIMIT - 1; //SET COORDINATES TO PLATFORM HEAD AT COORDINATES
	//uint16_t y = Y_SOFT_LIMIT - 1;
	//uint16_t z = 7000;
	//int i;
	while(1){
		ioboard_keypad_rl_get_key(&updown, 100);
		if (updown == 15) {
			//led->num = 8;
			platform_head_set_coords(X_SOFT_LIMIT - 1, Y_SOFT_LIMIT - 1, 7000);
			//x++;
			//ioboard_lcd_clear_display();
			///printf(buffer2, "%s", platform_head_at_coords());
			//ioboard_lcd_write_ascii("Coordinates", 0x40);
			//wait(1);
			//updown = 0;
		} else if (updown == 14) {
			//x--;
			//platform_head_set_coords(x, y, z);
			//updown = 0;
			
		} else if (updown == 8) {
			ioboard_lcd_clear_display();
			ioboard_lcd_write_ascii("Mode */# change", 0x00);
			ioboard_lcd_write_ascii(&a[count], 0x40);
			return;
		}
		updown = 0;
		wait(3);
		
	}
}

void yaxis(void){
	led_clear();
	//DISPLAY Position as well
	//MOVE axis X Direction
	while(1){
		ioboard_keypad_get_key(&updown);
		if (updown == 15) {
			led_clear();
			led_disp_num(1);
		} else if (updown == 14) {
			led_clear();
			led_disp_num(2);
		} else if (updown == 8) {
			ioboard_lcd_clear_display();
			ioboard_lcd_write_ascii("Mode */# change", 0x00);
			ioboard_lcd_write_ascii(&a[count], 0x40);
			return;
		}
	}
}

void zaxis(void){
	led_clear();
	//DISPLAY Position as well
	//MOVE axis X Direction
	while(1){
		ioboard_keypad_get_key(&updown);
		if (updown == 15) {
			led_clear();
			led_disp_num(1);
		} else if (updown == 14) {
			led_clear();
			led_disp_num(2);
		} else if (updown == 8) {
			ioboard_lcd_clear_display();
			ioboard_lcd_write_ascii("Mode */# change", 0x00);
			ioboard_lcd_write_ascii(&a[count], 0x40);
			return;
		}
	}
}


void manualmove(void){
	while(1){
		ioboard_keypad_rl_get_key(&postemp, 100);
		if (postemp == 7){ //num 3 on keypad
			xpos = 3;
		}
	}



}
