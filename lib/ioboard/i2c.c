#include "lpc17xx_pinsel.h"
#include "ioboard.h"

// Initialise the i2c bus to which the ioboard peripherals attached
//
// Returns:
//	void
//
// Parameters:
//	void
//      

I2C_M_SETUP_Type M_setup;

void ioboard_i2c_init(void) {
    PINSEL_CFG_Type i2c1_select;

    i2c1_select = (PINSEL_CFG_Type){
        .Funcnum = 3,
        .OpenDrain = PINSEL_PINMODE_NORMAL,
        .Pinmode = PINSEL_PINMODE_PULLUP,
        .Pinnum = 0,
        .Portnum = 0
    };

    PINSEL_ConfigPin(&i2c1_select);
    i2c1_select.Pinnum = 1;
    PINSEL_ConfigPin(&i2c1_select);

    I2C_Init(LPC_I2C1, 100000);
    I2C_Cmd(LPC_I2C1, ENABLE);
}


// Set the desired address to work with on i2c
//
// Returns:
//	void
//
// Parameters:
//	i2c address
//   

void i2c_address(int address) {
	M_setup.addr7bit = address;
}


// Write data using i2c bus
//
// Returns:
//	SUCCESS if write was performed
//	FAIL if write failed
//
// Parameters:
//	data - data to write
//	length - size of data in bytes
//   

int i2c_write(int data, int length) {
	M_setup.tx_data = &data;
	M_setup.tx_length = length;
	M_setup.rx_length = 0;
	return I2C_MasterTransferData(LPC, &M_setup, I2C_TRANSFER_POLLING)
}


// Read data from i2c bus
//
// Returns:
//	SUCCESS if read was performed
//	FAIL if read failed
//
// Parameters:
//	buffer - buffer to read data into
//	length - the size of the buffer in bytes
//   

int i2c_read(int buffer, int length) {
	M_setup.rx_length = length;
	M_setup.rx_data = &buffer;
	M_setup.tx_length = 0;
	return I2C_MasterTransferData(LPC, &M_setup, I2C_TRANSFER_POLLING)
}


// Combined protocol- first write data, after which read will be performed
//
// Returns:
//	SUCCESS if read and write was performed
//	FAIL if read and write failed
//
// Parameters:
//	write_data - data to write
//	write_length - size of data in bytes
//	read_data - buffer to read data into
//	read_length - the size of the buffer in bytes

int i2c_combined(int read_data, int write_data, int read_length, int write_length) {
	M_setup.tx_data = &write_data;
	M_setup.tx_length = write_length;
	M_setup.rx_data = &read_data;
	M_setup.rx_length = read_length;
	return I2C_MasterTransferData(LPC, &M_setup, I2C_TRANSFER_POLLING)
}

