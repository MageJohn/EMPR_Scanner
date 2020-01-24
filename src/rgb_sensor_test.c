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
 
#define rgb_scanner 0x29
#define LPC LPC_I2C1
#define sensor_size 8

// Registers
#define register_enable 0x00
#define register_atime 0x01

char buffer[sensor_size] = "y";
char receive_data[sensor_size] = "";
char send_data[sensor_size] = "0x00"; // finish this !

// 0 01 0 then address

int read_usb_serial_none_blocking(char *buf,int length);
int write_usb_serial_blocking(char *buf,int length);
void serial_init(void);
int delay(void);

int main(void) {

	serial_init();

	I2C_Init(LPC, 100000);
	I2C_Cmd(LPC, ENABLE);

	// Setup I2C SETUP_TYPE
	I2C_M_SETUP_Type M_setup;
	M_setup.tx_length = sensor_size;
	M_setup.tx_data = &send_data;
	M_setup.sl_addr7bit = rgb_scanner;
	M_setup.rx_data = &receive_data;
	M_setup.rx_length = sensor_size;

	write_usb_serial_blocking("\033[2J", 4);
	write_usb_serial_blocking("\033[H", 4);

	sprintf(buffer, "test");
	write_usb_serial_blocking(buffer, sensor_size);

	delay();

	while(1) {

		if (I2C_MasterTransferData(LPC, &M_setup, I2C_TRANSFER_POLLING) == SUCCESS) {

			sprintf(buffer, "o");
			write_usb_serial_blocking(buffer, sensor_size);		

			delay();			
			
		}
		
	}

}

int read_usb_serial_none_blocking(char *buf,int length) {
	return(UART_Receive((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)buf, length, NONE_BLOCKING));
}

// Write option board uses I2C1 (on port 0!)  which requires alternate function3 at pins P0.0 (mbed 9 / MPU 46) and P0.1 (mbeds
int write_usb_serial_blocking(char *buf,int length) {
	return(UART_Send((LPC_UART_TypeDef *)LPC_UART0,(uint8_t *)buf,length, BLOCKING));
}

// init code for the USB serial line
void serial_init(void)
{
	UART_CFG_Type UARTConfigStruct;			
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;	
	PINSEL_CFG_Type PinCfg;				

	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;

	// USB serial first
	PinCfg.Portnum = 0;
	
	// Screen 
	PinCfg.Pinnum = 2;	//Ci2c peripheralurrent Re-Transmission counter 
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);
		

	// I2C
	PinCfg.Funcnum = 3;
	PinCfg.Pinnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);

	UART_ConfigStructInit(&UARTConfigStruct);

	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);		// Initialize UART0 peripheral with given to 
	UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct);	// Initialize FIFO for UART0 peripheral
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);			// Enable UART Transmit
	

}

// Stalls the program for about 0.5 seconds
int delay(void) {

	int y = 0;
	int x;

	for (x = 0; x<5000000; x++) {
		y++;
	}

}
