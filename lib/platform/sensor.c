#include <string.h>
#include "time.h"

#include "leds.h"
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
static uint8_t atime = 0xff;
static bool enable;
static struct LedSource *led;


/*-------------------
 * Public functions
 * ------------------*/

void platform_sensor_get_data(uint16_t *buffer) {
    uint8_t set_reg = CMD | REG_CDATA;
    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = RGB_SENSOR_ADDR,
        .tx_data = &set_reg,
        .tx_length = 1,
        .rx_data = (uint8_t *)buffer,
        .rx_length = 8,
    };
    platform_i2c_transfer_blocking(&packet);
}

void platform_sensor_set_gain(enum SensorGain gain) {
    if (enable) {
        uint8_t data[] = {CMD | REG_CONTROL, gain};
        platform_i2c_write(RGB_SENSOR_ADDR, data, 2);
    }
}

void platform_sensor_set_integ_cycles(uint8_t cycles) {
    if (cycles < 1) {
        return;
    }
    atime = 256 - cycles;
    uint8_t data[] = {CMD | REG_ATIME, atime};
    platform_i2c_write(RGB_SENSOR_ADDR, data, 2);
}

void platform_sensor_wait_for_integration(void) {
    wait_us(2 * (2400 + 2400 * (256 - atime)));
}


/*---------------------------
 * Package private functions
 * --------------------------*/

void sensor_init(void){
    uint8_t test_data = 1;
    if (platform_i2c_write(RGB_SENSOR_ADDR, &test_data, 1) == SUCCESS) {
        enable = true;

    // Enable chip
    uint8_t tx_data[] = {CMD | REG_ENABLE,
                         ENABLE_PON | ENABLE_AEN,
                         atime};
    platform_i2c_write(RGB_SENSOR_ADDR, tx_data, 3);

    // Ensure power on process is complete before continuing.
    wait_us(2400);
}
