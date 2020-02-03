#include "lpc17xx_pinsel.h"
#include "ioboard.h"

#define LPC LPC_I2C1

// Initialise the i2c bus to which the ioboard peripherals attached
//
// Returns:
//      void
//
// Parameters:
//      void

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
//      void
//
// Parameters:
//      i2c address

void ioboard_i2c_address(uint8_t address) {
        M_setup.sl_addr7bit = address;
}


// Write a byte using i2c bus
//
// Returns:
//      SUCCESS if write was performed
//      ERROR if write failed
//
// Parameters:
//      data - data byte to write

Status ioboard_i2c_write_byte(uint8_t data) {
    return ioboard_i2c_combined(NULL, &data, 0, 1);
}


// Write data using i2c bus
//
// Returns:
//      SUCCESS if write was performed
//      ERROR if write failed
//
// Parameters:
//      data - A pointer to the data to write
//      length - The number of bytes to write

Status ioboard_i2c_write(uint8_t *data, uint32_t length) {
    return ioboard_i2c_combined(NULL, data, 0, length);
}


// Read a byte from i2c bus
//
// Returns:
//      SUCCESS if read was performed
//      ERROR if read failed
//
// Parameters:
//      variable - variable to read data into

Status ioboard_i2c_read_byte(uint8_t *variable) {
    return ioboard_i2c_combined(variable, NULL, 1, 0);
}


// Read data from i2c bus
//
// Returns:
//      SUCCESS if read was performed
//      ERROR if read failed
//
// Parameters:
//      buffer - a pointer to a buffer to read data into
//      length - the size of the buffer in bytes

Status ioboard_i2c_read(uint8_t *buffer, uint32_t length) {
    return ioboard_i2c_combined(buffer, NULL, length, 0);
}


// Read byte from a register over i2c
//
// Write a register address and then read a byte.
//
// Returns:
//      SUCCESS if read was performed
//      ERROR if read failed
//
// Parameters:
//      reg - the register to read from
//      variable - a pointer to store the byte to

Status ioboard_i2c_read_register_byte(uint8_t reg, uint8_t *variable) {
    return ioboard_i2c_combined(variable, &reg, 1, 1);
}


// Read data from a register over i2c
//
// Write a register address and then read.
//
// Returns:
//      SUCCESS if read was performed
//      ERROR if read failed
//
// Parameters:
//      reg - the register to read from
//      buffer - a pointer to store the data to
//      length - the size of the buffer in bytes

Status ioboard_i2c_read_register(uint8_t reg, uint8_t *buffer, uint32_t length) {
    return ioboard_i2c_combined(buffer, &reg, length, 1);
}


// Combined protocol- first write data, after which read will be performed
//
// Returns:
//      SUCCESS if read and write was performed
//      ERROR if read and write failed
//
// Parameters:
//      write_data - data to write
//      write_length - size of data in bytes
//      read_data - buffer to read data into
//      read_length - the size of the buffer in bytes

Status ioboard_i2c_combined(uint8_t *read_data, uint8_t *write_data,
                         uint32_t read_length, uint32_t write_length) {
    if (M_setup.sl_addr7bit) {
        M_setup.tx_data = write_data;
        M_setup.tx_length = write_length;
        M_setup.rx_data = read_data;
        M_setup.rx_length = read_length;
        return I2C_MasterTransferData(LPC, &M_setup, I2C_TRANSFER_POLLING);
    } else {
        return ERROR;
    }
}

