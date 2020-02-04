#include <string.h>
#include "ioboard.h"
#include "mbed.h"

#include "platform.h"
#include "sensor.h"

// File private defines
#define RGB_SENSOR_ADDR (0x29)

#define CMD (0xa0)
#define REG_CDATA (0x14)
#define REG_ENABLE (0x00)
#define REG_ATIME (0x01)
#define REG_CONTROL (0x0F)

#define ENABLE_AEN (0x2)
#define ENABLE_PON (0x1)

// A union which helps convert between a 16 bit int and two 8 bit bytes
	union ColourData {
    uint16_t combined[4];
    uint8_t low_high[8];
};

// File private function declarations
static uint16_t integration_time(void);


// File private variables
static union ColourData data;
static uint8_t atime = 0xFF;
static enum SensorGain again = GAIN1x;
static uint32_t time;
static bool enable;


/*-------------------
 * Public functions
 * ------------------*/

void platform_sensor_get_data(uint16_t *buffer) {
    memcpy((void *) buffer, (void *)data.combined, 8);
}

void platform_sensor_set_gain(enum SensorGain gain) {
    again = gain;
    ioboard_i2c_address(RGB_SENSOR_ADDR);
    ioboard_i2c_write((uint8_t[]){CMD | REG_CONTROL, (uint8_t)again}, 2);
}

void platform_sensor_set_integ_cycles(uint8_t cycles) {
    atime = 0xFF - cycles;

    ioboard_i2c_address(RGB_SENSOR_ADDR);
    ioboard_i2c_write((uint8_t[]){CMD | REG_ATIME, atime}, 2);
}

void platform_sensor_set_state(FunctionalState new_state) {
    enable = new_state;
}


/*---------------------------
 * Package private functions
 * --------------------------*/

void sensor_init(void){
    enable = true;

    ioboard_i2c_address(RGB_SENSOR_ADDR);

    // Enable chip
    uint8_t tx_data[] = {CMD | REG_ENABLE,
                         ENABLE_PON | ENABLE_AEN};
    ioboard_i2c_write(tx_data, 2);

    // Set address to clear high, (with auto-increment)
    ioboard_i2c_write_byte(CMD | REG_CDATA);

    // Ensure power on process is complete before continuing.
    wait_us(2400);
}

void sensor_tick(void) {
    if (enable && micros() - time > integration_time()){
        ioboard_i2c_address(RGB_SENSOR_ADDR);
        ioboard_i2c_read(data.low_high, 8);
    }
}


/*------------------------
 * File private functions
 * -----------------------*/

static uint16_t integration_time(void) {
    return 2400 + (2400 * (256 - atime));
}
