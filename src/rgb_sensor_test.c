#include "lpc17xx_i2c.h"
#include "leds.h"
#include <stdio.h>
#include <lpc17xx_i2c.h>
#include "lpc17xx_gpio.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "serial.h"	
#include <serial.h>
#include <math.h>
#include <ioboard.h>
#include "ioboard.h"
 
#define RGB_SCANNER 0x29
#define LPC LPC_I2C1
#define SENSOR_SIZE 8
#define SEND_SIZE_INIT 16
#define SEND_SIZE_RGB 8
#define BUF_SIZE 256

char buffer[BUF_SIZE];			// Readout values
char read_one[1];
uint8_t receive_data[SENSOR_SIZE];	// I2C read values
uint8_t send_data_init = 0x8003;	// Writes to REGISTER_ENABLE
uint8_t send_data_rgb = 0xb4;		// Reads registers 0x14 -> 0x1B

// RGB and clear 16-bit values
uint16_t red;
uint16_t green;
uint16_t blue;
uint16_t clear;

uint8_t init_counter = 0;
uint8_t counter = 0;
float clear_per;
float gain = 1;

uint8_t rgb_red;
uint8_t rgb_green;
uint8_t rgb_blue;
uint8_t rgb_clear;

int main(void) {

	serial_init();
	ioboard_i2c_init();

	// One random reading boi
	I2C_M_SETUP_Type M_setup;
	M_setup.sl_addr7bit = RGB_SCANNER;
	M_setup.tx_data = 0;
	M_setup.rx_length = 1;
	M_setup.rx_data = &read_one;
	I2C_MasterTransferData(LPC, &M_setup, I2C_TRANSFER_POLLING)
	

	// Setup I2C SETUP_TYPE for init
	M_setup.tx_length = 2;
	M_setup.tx_data = &send_data_init;
	M_setup.rx_length = 0;

	serial_write_b("\033[2J", 4);
	serial_write_b("\033[H", 4);

	sprintf(buffer, "Status: ");
	serial_write_b(buffer, BUF_SIZE);

	delay();


	// Write to ENABLE and ATIME registers

	while(init_counter < 1) {

		if (I2C_MasterTransferData(LPC, &M_setup, I2C_TRANSFER_POLLING) == SUCCESS) {

			sprintf(buffer, "Ok \n\r");
			serial_write_b(buffer, BUF_SIZE);

		} else {

			sprintf(buffer, "Writing failed. Terminating.");
			serial_write_b(buffer, BUF_SIZE);
			exit(0);

		}

		init_counter += 1;

	}

	delay();

	// Setup I2C for writing then reading data
	M_setup.tx_length = SEND_SIZE_RGB;
	M_setup.tx_data = &send_data_rgb;
	M_setup.rx_data = &receive_data;
	M_setup.rx_length = SENSOR_SIZE;
	

	while(1) {

		gain = 1;

		if (counter == 8) {

			M_setup.tx_data = &send_data_rgb;
			M_setup.rx_data = &receive_data;
			counter = 0;

		}

		if (I2C_MasterTransferData(LPC, &M_setup, I2C_TRANSFER_POLLING) == SUCCESS) {

			// Clear screen
			serial_write_b("\033[2J", 4);
			serial_write_b("\033[H", 4);

			// Combine values into 16-bit integers. First input is the high byte, second-low byte
			clear = (receive_data[1] << 8) | receive_data[0];
			red = (receive_data[3] << 8) | receive_data[2];
			green = (receive_data[5] << 8) | receive_data[4];
			blue = (receive_data[7] << 8) | receive_data[6];
			

			// Gain
			clear_per = clear/65536;
			gain += clear_per;


			// To rgb

			if (clear == 0) {

				red, green, blue = 0;

			}

			float inter_red = ((((float)red/(float)clear)) * 256.0) / 255.0;
			float inter_green = ((((float)green/(float)clear)) * 256.0) / 255.0;
			float inter_blue = ((((float)blue/(float)clear)) * 256.0) / 255.0;


			rgb_red = (int)(powf(inter_red, 2.5) * 255.0);
			rgb_green = (int)(powf(inter_green, 2.5) * 255.0);
			rgb_blue = (int)(powf(inter_blue, 2.5) * 255.0);


			// Apply gain

			rgb_red *= gain;
			rgb_green *= gain;
			rgb_blue *= gain;

			if (red > 255) {
				red = 255;
			}

			if (green > 255) {
				green = 255;
			}

			if (blue > 255) {
				blue = 255;
			}	

			// For hex value: 0x%02x
			//sprintf(buffer, "Values: RL %d RH %d GL %d GH %d BL %d BH %d", receive_data[3], receive_data[4], receive_data[5], receive_data[6], receive_data[7], receive_data[8]);
			int buffer_length = sprintf(buffer, "Values:  Red %d    Green %d    Blue %d\n\rClear value: %d", rgb_red, rgb_green, rgb_blue, clear);
			serial_write_b(buffer, buffer_length);	

			counter = counter + 1;
			delay();

		}		
	}

}

// Stalls the program for about 0.050 seconds
int delay(void) {

	int y = 0;
	int x;

	for (x = 0; x<500000; x++) {
		y++;
	}

}
