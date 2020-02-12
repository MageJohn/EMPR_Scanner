#include "platform.h"
#include "mbed.h"
#include <stdlib.h>
#include "raster_scan_B2.h"
#include "serial.h"

#define Y_START 400
#define X_START 280
#define Z_START 200
#define X_RES 50
#define Y_RES 25

static uint64_t rgb_vals[Y_RES];

static void init_send_data(void);

void main(void) {
    platform_init();
    platform_calibrate_head();
    //platform_sensor_set_integ_cycles(10);
    while(!platform_calibrated());
    init_send_data();
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
    platform_head_set_coords(x, y, z);
    while(!platform_head_at_coords());
    platform_sensor_get_data(&rgb_vals[0]);
    for(i = 1; i < no_steps; i++) {
        y += step;
        platform_head_set_coords(x, y, z);
        while(!platform_head_at_coords());
        wait_ms(5);
        platform_sensor_get_data(&rgb_vals[i]);
    }
}

//x_res and y_res conceptually number of 'pixels'. Choose as a factor of limits!
void raster_scan(int16_t x, int16_t y, int16_t z, int16_t x_res, int16_t y_res) {
    int16_t step = (Y_SOFT_LIMIT - y)/y_res;
    int16_t next_row = (X_SOFT_LIMIT - x)/x_res;
    int i;
    uint16_t start_y = y;
    for(i = 0; i < x_res; i++) {
        one_dimensional_scan_y(x, y, z, step);
        serial_write_b((char *)rgb_vals, sizeof(uint64_t) * Y_RES);
        x += next_row;
        step *= -1;

        if(y == start_y) {
            y = Y_SOFT_LIMIT;
        }
        else {
            y = start_y;
        }
    }
}

void init_send_data(void) {
    serial_init();
    uint16_t buffer1 = X_RES;
    uint16_t buffer2 = Y_RES;
    serial_write_b((char *)&buffer1, sizeof(uint16_t));
    serial_write_b((char *)&buffer2, sizeof(uint16_t));
}
