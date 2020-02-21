#include "platform.h"
#include "time.h"
#include <stdlib.h>
#include "serial.h"

#define Z 30
#define X_END 800
#define X_START 300
#define Y_END 850
#define Y_START 350
#define X_RES 50
#define Y_RES 50

static uint64_t rgb_vals;

static void init_send_data(void);
void one_dimensional_scan_y(int16_t x, int16_t y, int16_t z, int8_t step, int16_t stop);

void init_send_data(void) {
    serial_init();
    uint16_t buffer1 = X_RES;
    uint16_t buffer2 = Y_RES;
    serial_write_b((char *)&buffer1, sizeof(uint16_t));
    serial_write_b((char *)&buffer2, sizeof(uint16_t));
}

void one_dimensional_scan_y(int16_t x, int16_t y, int16_t z, int8_t step, int16_t stop) {
    int16_t no_steps;
    if(y == stop) {
        no_steps = abs((stop - Y_START)/step);
    }
    else{
        no_steps = abs((stop - y)/step);
    }
    int i;
    for (i = 0; i < no_steps; i++) {
        //wait_us((2400 + (2400 * INTEG_CYCLES)) * 2);
        platform_sensor_get_data((uint16_t *)&rgb_vals);
        platform_head_set_coords(x, y, z);
        serial_write_b((char *)&rgb_vals, sizeof(uint64_t));
        while(!platform_head_at_coords());
        y += step;
    }
}

void main(void) {
    platform_init();
    serial_init();
    init_send_data();

    platform_calibrate_head();
    while (!platform_calibrated());

    int16_t step = (Y_END - Y_START)/Y_RES;
    int16_t next_row = (X_END - X_START)/X_RES;
    uint16_t y = Y_START;
    uint16_t x = X_START;
    int8_t i;
    platform_head_set_coords(x, y, Z);
    while(!platform_head_at_coords());
    for(i = 0; i < X_RES; i++) {
        one_dimensional_scan_y(x, y, Z, step, Y_END);
        y = (y == Y_START) ? Y_END : Y_START;
        step *= -1;
        x += next_row;
    }
}
