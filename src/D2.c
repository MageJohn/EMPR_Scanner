#include "scanning.h"
#include "platform.h"
#include "time.h"
#include <stdlib.h>
#include "serial.h"

int16_t x;
int16_t y;
int16_t z;

uint64_t crgb;

void main(void) {
    platform_init();
    serial_init();

    platform_calibrate_head();
    while(!platform_calibrated());

     while (true) {

        serial_read_b((char *)&x, 2);
        serial_read_b((char *)&y, 2);
        serial_read_b((char *)&z, 2);

        platform_head_set_coords(x, y, z);
        while(!platform_head_at_coords());

        platform_sensor_get_data((uint16_t *)&crgb);
        serial_write_nb((char *)&crgb, sizeof(uint64_t));
     }
}