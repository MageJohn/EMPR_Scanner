#include "platform.h"
#include "serial.h"
#include "leds.h"

#define Z 40
#define X_END 920
#define X_START 150
#define Y_END 850
#define Y_START 150
#define X_RES 100
#define Y_RES 100
#define INTEG_CYCLES 1
#define WAIT false
#define CAL_XY true
#define CAL_XY_FREQ 5
#define CAL_Y true
#define TWO_WAY false

static uint64_t rgb_vals;

static void init_send_data(void);
void one_dimensional_scan_y(int16_t x, int16_t y, int16_t z, int8_t step, int16_t stop);

void init_send_data(void) {
    uint16_t res[] = {X_RES, Y_RES};
    char start;
    serial_init();

    serial_read_b(&start, 1);

    serial_write_b((char *)&res, sizeof(res));
}

void one_dimensional_scan_y(int16_t x, int16_t y, int16_t z, int8_t step, int16_t stop) {
    for (; y != stop; y += step) {
        platform_head_set_coords(x, y, z);
        while(!platform_head_at_coords());
        if (WAIT) {
            platform_sensor_wait_for_integration();
        }
        platform_sensor_get_data((uint16_t *)&rgb_vals);
        while(!serial_nb_write_finished());
        serial_write_nb((char *)&rgb_vals, sizeof(uint64_t));
    }
}

static void two_way_raster(int16_t x_step, int16_t y_step, int16_t x_stop, int16_t y_stop) {
    int16_t raster_end = y_stop - y_step;
    int16_t raster_begin = Y_START;
    int16_t y = raster_begin;
    int16_t x = X_START;

    for(; x != x_stop; x += x_step) {
        one_dimensional_scan_y(x, y, Z, y_step, (y + Y_RES * y_step));

        if (y == raster_end) {
            y = raster_begin;
            if (CAL_XY && (x / x_step) % CAL_XY_FREQ == 0) {

                platform_head_set_coords(-10, -10, Z);
            } else if (CAL_Y) {
                platform_head_set_coords(x, -10, Z);
            }
            while(!platform_head_at_coords());
        } else {
            y = raster_end;
        }
        y_step *= -1;
    }
}

void raster(int16_t x_step, int16_t y_step, int16_t x_stop, int16_t y_stop) {
    int16_t x = X_START;
    for (; x != x_stop; x += x_step) {
        one_dimensional_scan_y(x, Y_START, Z, y_step, y_stop);
        if (CAL_XY && (x / x_step) % CAL_XY_FREQ == 0) {
            platform_head_set_coords(-10, -10, Z);
            while(!platform_head_at_coords());
        } else if (CAL_Y) {
            platform_head_set_coords(x, -10, Z);
            while(!platform_head_at_coords());
        }
    }
}


void main(void) {
    int16_t y_step = (Y_END - Y_START)/Y_RES;
    int16_t x_step = (X_END - X_START)/X_RES;
    int16_t y_stop = (Y_START + Y_RES * y_step);
    int16_t x_stop = (X_START + X_RES * x_step);

    led_setup();
    platform_init();
    led_mux_set_curr(HB_LED);

    platform_calibrate_head();
    init_send_data();
    platform_sensor_set_integ_cycles(INTEG_CYCLES);
    while (!platform_calibrated());

    if (TWO_WAY) {
        two_way_raster(x_step, y_step, x_stop, y_stop);
    } else {
        raster(x_step, y_step, x_stop, y_stop);
    }
}
