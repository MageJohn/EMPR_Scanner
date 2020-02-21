#include "platform.h"
#include "time.h"
#include <stdlib.h>
#include "serial.h"
#include "leds.h"

#define Y_START 400
#define X_START 280
#define Z_START 30
#define X_RES 38
#define Y_RES 25

static uint64_t rgb_vals[Y_RES];

static void init_send_data(void);

void raster_scan(int16_t x, int16_t y, int16_t z, int16_t grid_size_x, int16_t grid_size_y);
void one_dimensional_scan_y(int16_t x, int16_t y, int16_t z, int8_t step);

struct LedSource *led;

void main(void) {
    led_setup();
    led = led_mux_register_source(1000);
    platform_init();
    led_mux_set_curr(1000);
    platform_calibrate_head();
    led->num = 1;
    while(!platform_calibrated());
    led->num += 1;
    init_send_data();
    led->num += 1;

    raster_scan(X_START, Y_START, Z_START, X_RES, Y_RES);
}

//step is no of motor revolutions.
void one_dimensional_scan_y(int16_t x, int16_t y, int16_t z, int8_t step) {
    int16_t no_steps;
    if(y == Y_SOFT_LIMIT) {
        no_steps = abs((Y_SOFT_LIMIT - Y_START)/step);
    }
    else{
        no_steps = abs((Y_SOFT_LIMIT - y)/step);
    }
    int i;
    for (i = 0; i < no_steps; i++) {
        platform_head_set_coords(x, y, z);
        while(!platform_head_at_coords());
        platform_sensor_get_data((uint16_t *)&rgb_vals[i]);
        y += step;
    }
}

//x_res and y_res conceptually number of 'pixels'. Choose as a factor of limits!
void raster_scan(int16_t x, int16_t y, int16_t z, int16_t x_res, int16_t y_res) {
    int16_t step = (Y_SOFT_LIMIT - y)/y_res;
    int16_t next_row = (X_SOFT_LIMIT - x)/x_res;
    uint16_t start_y = y;
    led->num += 1;
    for(; x < X_SOFT_LIMIT; x += next_row) {
        led->num += 1;
        one_dimensional_scan_y(x, y, z, step);
        led->num += 1;
        serial_write_b((char *)rgb_vals, sizeof(uint64_t) * Y_RES);
        y = (y == start_y) ? Y_SOFT_LIMIT : start_y;
        step *= -1;
    }
}

void init_send_data(void) {
    serial_init();
    uint16_t buffer1 = X_RES;
    uint16_t buffer2 = Y_RES;
    serial_write_b((char *)&buffer1, sizeof(uint16_t));
    serial_write_b((char *)&buffer2, sizeof(uint16_t));
}
