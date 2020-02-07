#include "platform.h"
#include "mbed.h"
#include <stdlib.h>
#include "raster_scan_B2.h"
#include "serial.h"

#define Y_START 100
#define X_START 230
#define Z_START 15
#define X_RES 2
#define Y_RES 10

static rgb_vals[X_RES][Y_RES];

void main(void) {
    platform_init();
    platform_calibrate_head();
    while(!platform_calibrated());
    raster_scan(X_START, Y_START, Z_START, X_RES, Y_RES);
    send_data();
}

//step is no of motor revolutions.
void one_dimensional_scan_y(int16_t x, int16_t y, int16_t z, int8_t step, uint16_t *rgb_array) {
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
    for(i = 0; i < no_steps; i++) {
        y += step;
        platform_head_set_coords(x, y, z);
        while(!platform_head_at_coords());
        //rgb_array[i] = sensor_read();
        wait_ms(50);
    }
}

//x_res and y_res conceptually number of 'pixels'. Choose as a factor of limits!
void raster_scan(int16_t x, int16_t y, int16_t z, int16_t x_res, int16_t y_res) {
    int16_t step = (Y_SOFT_LIMIT - y)/y_res;
    int16_t next_row = (X_SOFT_LIMIT - x)/x_res;
    int i;
    int16_t start_y = y;
    for(i = 0; i < x_res; i++) {
        one_dimensional_scan_y(x, y, z, step, rgb_vals[i]);
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

void send_data(void) {
    serial_init();
    uint16_t buffer;
    uint16_t buffer1 = X_RES;
    uint16_t buffer2 = Y_RES;
    serial_write_b(&buffer1, 2);
    serial_write_b(&buffer2, 2);
    int i;
    for(i = 0; i < X_RES; i++) {
        buffer = rgb_vals[i];
        serial_write_b(buffer, 2*Y_RES);
    }
}

