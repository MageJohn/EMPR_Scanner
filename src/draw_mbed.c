/* The draw function will always be given an image of 200x200 to draw. 
   To make this centered, we will 
*/
#include "scanning.h"
#include "platform.h"
#include "time.h"
#include <stdlib.h>
#include "serial.h"

#define OFFSET 400
#define SCALE 10

int16_t x;
int16_t y;
int16_t z;

void main(void) {
    int16_t last_z = 1000;
    platform_init();
    serial_init();

    struct ScanningConfig cfg = {
        .z = 40,
        .start = {200, 350},
        .size = {750, 500},
        .res = {75, 50},
        .cal_freqs = {0, 1},
        .wait_for_sensor = false,
        .send_data = true,
        .show_lcd = false,
    };

    platform_calibrate_head();
    while(!platform_calibrated());

    scanning_setup(&cfg);
    serial_wait_for_byte();
    scanning_raster(X, Y);

    platform_calibrate_head();
    while(!platform_calibrated());

    platform_head_set_coords(0,0,1000);
    while(!platform_head_at_coords());

    serial_wait_for_byte();

    while (true) {

        serial_read_b((char *)&x, 2);
        serial_read_b((char *)&y, 2);
        serial_read_b((char *)&z, 2);

        x *= SCALE;
        y *= SCALE;

        platform_head_set_coords(x+OFFSET, y+400, z);
        while(!platform_head_at_coords());

        if (z - last_z > 0) {
            platform_head_set_coords(-X_SOFT_LIMIT, -Y_SOFT_LIMIT, z);
            while(!platform_head_at_coords());
            platform_head_set_coords(x, y, z);
            while(!platform_head_at_coords());
        }

        last_z = z;

        serial_write_b("A", 1);
    }

}