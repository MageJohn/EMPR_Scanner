#include "platform.h"
#include "mbed.h"
#include "stdlib.h"
#include "raster_scan_B2.h"

#define Y_START 100
#define X_START 230
#define Z_START 15
#define X_RES 10
#define Y_RES 10

void main(void) {
    platform_init();
    platform_calibrate_head();
    while(!platform_calibrated());
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
    for(i = 1; i <= no_steps; i++) {
        y += step;
        platform_head_set_coords(x, y, z);
        while(!platform_head_at_coords());
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
        one_dimensional_scan_y(x, y, z, step);
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