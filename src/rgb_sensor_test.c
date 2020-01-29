#include "lpc17xx_i2c.h"
#include "ioboard/i2c.h"
#include "mbed/rit.h"
#include "leds.h"
#include "mbed/wait.h"
#include <stdio.h>
#include <lpc17xx_i2c.h>
#include "lpc17xx_gpio.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "serial.h"
#include <math.h>
 
#define RGB_SCANNER 0x29
#define LPC LPC_I2C1
#define SENSOR_SIZE 9
#define SEND_SIZE_INIT 16
#define SEND_SIZE_RGB 9
#define BUF_SIZE 256

char buffer[BUF_SIZE];				// Readout values
uint8_t receive_data[SENSOR_SIZE];	// I2C read values
uint8_t send_data_init = 0xa0;		// Writes to REGISTER_ENABLE and REGISTER_ATIME
uint8_t send_data_rgb = 0xb3;		// Reads registers 0x13 -> 0x1B

// RGB and clear 16-bit values
uint16_t red;
uint16_t green;
uint16_t blue;
uint16_t clear;


int main(void) {

	// TODO: Initiate RGBC timing register !
	// TODO: Send enable and Atime seperatly
	// TODO: GOogle why blue sensor is too high/sensitive

	serial_init();

	ioboard_i2c_init();

	// Setup I2C SETUP_TYPE for init
	I2C_M_SETUP_Type M_setup;
	M_setup.tx_length = 1;
	M_setup.tx_data = &send_data_init;
	M_setup.sl_addr7bit = RGB_SCANNER;
	M_setup.rx_length = 0;

	serial_write_b("\033[2J", 4);
	serial_write_b("\033[H", 4);

	sprintf(buffer, "Status: ");
	serial_write_b(buffer, BUF_SIZE);

	delay();


	// Write to ENABLE and ATIME registers
	if (I2C_MasterTransferData(LPC, &M_setup, I2C_TRANSFER_POLLING) == SUCCESS) {

		sprintf(buffer, "Writing succeeded");
		serial_write_b(buffer, BUF_SIZE);

	} else {

		sprintf(buffer, "Writing failed. Terminating.");
		serial_write_b(buffer, BUF_SIZE);
		exit(0);

	}

	delay();

	// Setup I2C for writing then reading data
	M_setup.tx_length = SEND_SIZE_RGB;
	M_setup.tx_data = &send_data_rgb;
	M_setup.rx_data = &receive_data;
	M_setup.rx_length = SENSOR_SIZE;
	

	while(1) {

		if (I2C_MasterTransferData(LPC, &M_setup, I2C_TRANSFER_POLLING) == SUCCESS) {

			// Clear screen
			serial_write_b("\033[2J", 4);
			serial_write_b("\033[H", 4);

			// TODO: Values not working correctly. Blue too high. Maybe overflow?

			// Combine values into 16-bit integers. First input is the high byte, second-low byte
			clear = (receive_data[2] << 8) | receive_data[1];
			red = (receive_data[4] << 8) | receive_data[3];
			green = (receive_data[6] << 8) | receive_data[5];
			blue = (receive_data[8] << 8) | receive_data[7];
			

			// For hex value: 0x%02x
			//sprintf(buffer, "Values: RL %d RH %d GL %d GH %d BL %d BH %d", receive_data[3], receive_data[4], receive_data[5], receive_data[6], receive_data[7], receive_data[8]);
			sprintf(buffer, "Values:  Red %d    Green %d    Blue %d\n\rClear value: %d", red, green, blue, clear);
			serial_write_b(buffer, BUF_SIZE);	

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

