#include <string.h>
#include "time.h"

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

// File private variables
static bool enable;
static uint8_t integ_cycles;


/*-------------------
 * Public functions
 * ------------------*/

void platform_sensor_get_data(uint16_t *buffer) {
    platform_i2c_read(RGB_SENSOR_ADDR, (uint8_t *)buffer, 8);
    //wait_us(2400 * integ_cycles);
}

void platform_sensor_set_gain(enum SensorGain gain) {
    uint8_t data[] = {CMD | REG_CONTROL, gain};
    platform_i2c_write(RGB_SENSOR_ADDR, data, 2);
}

void platform_sensor_set_integ_cycles(uint8_t cycles) {
    integ_cycles = cycles;
    uint8_t data[] = {CMD | REG_CONTROL, 0xFF - cycles};
    platform_i2c_write(RGB_SENSOR_ADDR, data, 2);
}


/*---------------------------
 * Package private functions
 * --------------------------*/

void sensor_init(void){
    enable = true;

    // Enable chip
    uint8_t tx_data[] = {CMD | REG_ENABLE,
                         ENABLE_PON | ENABLE_AEN};
    platform_i2c_write(RGB_SENSOR_ADDR, tx_data, 2);

    // Set address to clear high, (with auto-increment)
    tx_data[0] = CMD | REG_CDATA;
    platform_i2c_write(RGB_SENSOR_ADDR, tx_data, 1);

    // Ensure power on process is complete before continuing.
    wait_us(2400);
}
