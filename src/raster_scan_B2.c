
#include "platform.h"
#include "mbed.h"
#include "raster_scan_B2.h"
#include "leds.h"

// Ensure that step and end/start correlate so final step will end on end/start position
#define X_START 80
#define Y_START 50
#define Z_HEIGHT 10

struct LedSource *led;


int main(void) {
    platform_init();
    led_setup();
    led = led_mux_register_source(20);
    led_mux_set_curr(20);
    platform_calibrate_head();
    while(!platform_calibrated());
    raster_scan(X_START, Y_START, Z_HEIGHT, 100, 20);
    return(1);

}

//up to prgrammer to decide which dimension to scan. make stop_coord correspond.
//dimension chooses where to scan. 1 = x, 2 = y
bool one_dimensional_scan(uint16_t x, uint16_t y, uint16_t z, uint16_t stop_coord, uint8_t step) {
    platform_head_set_coords(x, y, z);
    while(!platform_head_at_coords());
    bool end = false;
    while(!end) {
        x += step;
        if(x >= stop_coord) {
            end = true;

        }
        else if (x <= stop_coord) {
            end = true;
        }
        platform_head_set_coords(x, y, z);
        while(!platform_head_at_coords());
        wait_ms(500);
        //do sensor stuff here
    }
    return true;
}


//void detect_flag(int16_t mid_x, uint16_t mid_y, uint16_t z, uint16_t *x_var, uint16_t *y_var) {}

void raster_scan(uint16_t x, uint16_t y, uint16_t z, uint16_t grid_size_x, uint16_t grid_size_y) {
    //starting_position(x, y, z);
    uint16_t start_x = x;
    uint16_t stop_x = X_SOFT_LIMIT;
    uint16_t x_step = (X_SOFT_LIMIT - x)/grid_size_x;
    uint16_t y_step = (Y_SOFT_LIMIT - y)/grid_size_y;
    while(true) {
        if (y >= Y_SOFT_LIMIT) {
            platform_head_set_coords(0, 0, 0);
            break;
        } else if(x == stop_x) {
            one_dimensional_scan(stop_x, y, z, start_x, x_step);
            x = start_x;
            x_step *= -1;
        }
        else {
            one_dimensional_scan(start_x, y, z, stop_x, x_step);
            x = stop_x;
            x_step *= -1;
        }
        y += y_step;
        led->num = y/10;
        //platform_head_set_coords(x,y,z);
        //while(!platform_head_at_coords());
    }
}