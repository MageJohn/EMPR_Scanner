#include "lpc_types.h"
#include "lpc17xx_i2c.h"

#include <stdbool.h>

// Debug led sources
#define LED_BASE (1000)
#define HB_LED (LED_BASE + 1)
#define X_LED (LED_BASE + 2)
#define Y_LED (LED_BASE + 3)
#define Z_LED (LED_BASE + 4)
#define RGB_LED (LED_BASE + 5)
#define SENSOR_LED (LED_BASE + 6)
#define I2C_LED (LED_BASE + 7)

// Axis limits
#define X_SOFT_LIMIT 980
#define Y_SOFT_LIMIT 850
#define Z_SOFT_LIMIT 7000


enum SensorGain {
    GAIN1x,
    GAIN4x,
    GAIN16x,
    GAIN60x
};

enum I2C_Status {
    FINISHED,
    RUNNING,
    FAILED,
};

// A union which helps convert between a 16 bit int and two 8 bit bytes
union ColourData {
    uint16_t combined[4];
    uint8_t low_high[8];
};

struct Transfer {
    I2C_M_SETUP_Type *packet;
    enum I2C_Status status;
    struct Transfer *next;
};


// Call once before calling other functions.
void platform_init(void);

// Call to have the head go to coordinates x, y, z
//
// Parameters:
//      uint16_t x, y, z: The coordinate to move to.
void platform_head_set_coords(uint16_t x, uint16_t y, uint16_t z);

// Whether the head is at the coordinates set by platform_head_set_coords.
//
// Returns:
//      bool: true if at the coordinates, false otherwise
bool platform_head_at_coords(void);

// Start the calibration process for the head position. Once called, wait until
// platform_calibrated returns true before calling other functions on the head. 
void platform_calibrate_head(void);

// Whether the platform has finished calibrating after a call to
// platform_calibrate_head.
//
// Returns:
//      bool: true if calibrated, false otherwise. 
bool platform_calibrated(void);

// Set the interval in milliseconds between motor ticks; higher values make the
// motors move more slowly.
//
// Parameters:
//      uint16_t interval: the interval to use
void platform_motor_update_interval(uint16_t new_interval);

// Copy the latest RGB sesnor data into the buffer passed.The buffer must have
// enough space to contain 4 16 bit integers.
//
// Parameters:
//      uint16_t *buffer: the location to copy the data to.
void platform_sensor_get_data(uint16_t *buffer);

// Set the gain of the sensor
//
// Parameters:
//      gain: the amount of gain
void platform_sensor_set_gain(enum SensorGain gain);


// Set the number of integration cycles for the chip.
//
// The time taken to update the data is 2.4 ms times the number of integration
// cycles.
void platform_sensor_set_integ_cycles(uint8_t cycles);

/*
  Schedule an I2C transfer and block until completion.

  Parameters: Same as for platform_i2c_schedule_transfer

  Returns:
    SUCCESS if the transfer was successfull
    ERROR if the transfer failed
*/
Status platform_i2c_transfer_blocking(I2C_M_SETUP_Type *packet);

/*
  Adds a new I2C transfer to be completed.

  Parameters:
    addr: the I2C address to initiate the transfer with
    tx_data: a pointer to the data to transmit
    tx_length: the number of bytes to transmit
    rx_data: a pointer to a buffer to read into
    rx_length: the number of bytes to read
  Returns:
    The id of the transaction, to be used with platform_i2c_transfer_status
*/
struct Transfer* platform_i2c_schedule_transfer(I2C_M_SETUP_Type *packet);

/*
  Remove a transfer from the linked list and free it's memory. If the transfer
  has not run, it will now never run.

  Parameters:
    transfer: A pointer to the transfer to remove
*/
void platform_i2c_remove_transfer(struct Transfer *transfer);

/*
  Write the length bytes from the location pointed to by data to addr

  Parameters:
    addr: the i2c address to write too
    data: a pointer to the data to write
    length: the number of bytes to write
  Returns:
    SUCCESS or ERROR
*/
Status platform_i2c_write(uint8_t addr, uint8_t *data, uint32_t length);

/*
  Read the length bytes from the addr into the location pointed to by buffer

  Parameters:
    addr: the i2c address to read from
    data: a pointer to a buffer to put the data in
    length: the number of bytes to read

  Returns:
    SUCCESS or ERROR
*/
Status platform_i2c_read(uint8_t addr, uint8_t *buffer, uint32_t length);

