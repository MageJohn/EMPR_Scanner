
#include "platform.h"
#include "mbed.h"
#include "raster_scan_B2.h"
#include "leds.h"
#include "serial.h"

// Ensure that step and end/start correlate so final step will end on end/start position
#define X_START 0
#define Y_START 0
#define Z_HEIGHT 10
#define X_RES 10
#define Y_RES 10

struct LedSource *led;

static uint16_t rgb_vals[X_RES][Y_RES];

int main(void) {
    platform_init();
    led_setup();
    led = led_mux_register_source(20);
    //led_mux_set_curr(25);
    platform_calibrate_head();
    while(!platform_calibrated());
    led_mux_set_curr(20);
    led->num = 8;
    raster_scan(X_START, Y_START, Z_HEIGHT, X_RES, Y_RES);
    display_data();
    return(1);

}

//up to prgrammer to decide which dimension to scan. make stop_coord correspond.
//dimension chooses where to scan. 1 = x, 2 = y
bool one_dimensional_scan(uint16_t x, uint16_t y, uint16_t z, uint8_t step, uint16_t *p_rgb_vals) {
    uint16_t count;
    uint16_t current_x = x;
    uint16_t sensor_val;
    for(count = 0; count < X_RES; count++) {
        current_x += step;
        platform_head_set_coords(current_x,y,z);
        while(!platform_head_at_coords());
        //senor_val = sensor_read();
        p_rgb_vals[count] = sensor_val;
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
    uint16_t current_y = y;
    uint16_t current_x = start_x;
    uint16_t current_z = z;
    uint16_t array_count = -1;
    while(true) {
        if(array_count == X_RES) {
        while(!platform_head_at_coords());
            array_count = -1;
        }
        array_count += 1;

        if (current_y >= Y_SOFT_LIMIT) {
            platform_head_set_coords(0, 0, 0);
            while(!platform_head_at_coords());
            break;
        // rgb_vals is an array declared in main(). maybe make this cleaner.
        } else if(current_x == stop_x) {
            one_dimensional_scan(stop_x, current_y, current_z, x_step, rgb_vals[array_count]);
            current_x = start_x;
            x_step *= -1;
            current_y += y_step;
            platform_head_set_coords(current_x,current_y,current_z);
            while(!platform_head_at_coords());
        }
        else  if(current_x == start_x) {
            one_dimensional_scan(start_x, current_y, current_z, x_step, rgb_vals[array_count]);
            current_x = stop_x;
            x_step *= -1;
            current_y += y_step;
            platform_head_set_coords(current_x,current_y,current_z);
            while(!platform_head_at_coords());
        }
    }
}

void display_data() {
    serial_init();
    uint16_t x;
    uint16_t y;
    uint16_t value;
    for(x = 0; x < X_RES; x++) {
        for(y = 0; y < Y_RES; y++) {
            value = rgb_vals[x][y];
            serial_write_b(value, 2);
            serial_write_b(" ",1);
        }
        serial_write_b("\r\n",2);
    } 
}
