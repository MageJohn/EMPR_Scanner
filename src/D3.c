#include "scanning.h"
#include "platform.h"
#include "time.h"
#include <stdlib.h>
#include "serial.h"

void main(void) {
    platform_init();
    serial_init();

    struct ScanningConfig cfg = {
        .z = 100,
        .start = {0, 0},
        .size = {980, 850},
        .res = {100, 100},
        .cal_freqs = {0, 1},
        .wait_for_sensor = false,
        .send_data = true,
        .show_lcd = false,
    };


    platform_calibrate_head();
    while(!platform_calibrated());

    scanning_setup(&cfg);
    serial_wait_for_byte();
    serial_write_b((char *)cfg.res, sizeof(uint16_t)*2);
    scanning_raster(X, Y);
}