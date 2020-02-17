#include "platform.h"
#include "mbed.h"
#include <stdlib.h>
#include "raster_scan_B2.h"
#include "serial.h"

#define X_ONE 275
#define X_TWO 475
#define X_THREE 675
#define Y_ONE 400
#define Y_TWO 600
#define Y_THREE 800
#define Z 25

#define X_RES 3
#define Y_RES 3

static uint64_t rgb_vals[9];

void main(void) {
    platform_init();
    serial_init();
    
    platform_calibrate_head();
    while (!platform_calibrated());
    
    platform_head_set_coords(X_ONE, Y_ONE, Z);
    while(!platform_head_at_coords());
    wait_ms(1000);
    platform_sensor_get_data(&rgb_vals[0]);
    
    platform_head_set_coords(X_TWO, Y_ONE, Z);
    while(!platform_head_at_coords());
    wait_ms(1000);
    platform_sensor_get_data(&rgb_vals[1]);
    
    platform_head_set_coords(X_THREE, Y_ONE, Z);
    while(!platform_head_at_coords());
    wait_ms(1000);
    platform_sensor_get_data(&rgb_vals[2]);

    platform_calibrate_head();
    while (!platform_calibrated());

    platform_head_set_coords(X_ONE, Y_TWO, Z);
    while(!platform_head_at_coords());
    wait_ms(1000);
    platform_sensor_get_data(&rgb_vals[3]);
    
    platform_head_set_coords(X_TWO, Y_TWO, Z);
    while(!platform_head_at_coords());
    wait_ms(1000);
    platform_sensor_get_data(&rgb_vals[4]);
    
    platform_head_set_coords(X_THREE, Y_TWO, Z);
    while(!platform_head_at_coords());
    wait_ms(1000);
    platform_sensor_get_data(&rgb_vals[5]);

    platform_calibrate_head();
    while (!platform_calibrated());

    platform_head_set_coords(X_ONE, Y_THREE, Z);
    while(!platform_head_at_coords());
    wait_ms(1000);
    platform_sensor_get_data(&rgb_vals[6]);
    
    platform_head_set_coords(X_TWO, Y_THREE, Z);
    while(!platform_head_at_coords());
    wait_ms(1000);
    platform_sensor_get_data(&rgb_vals[7]);
    
    platform_head_set_coords(X_THREE, Y_THREE, Z);
    while(!platform_head_at_coords());
    wait_ms(1000);
    platform_sensor_get_data(&rgb_vals[8]);

    uint16_t buffer1 = X_RES;
    uint16_t buffer2 = Y_RES;
    serial_write_b((char *)&buffer1, sizeof(uint16_t));
    serial_write_b((char *)&buffer2, sizeof(uint16_t));

    serial_write_b((char *)rgb_vals, sizeof(uint64_t) * (Y_RES * X_RES));
}
