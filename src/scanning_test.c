#include "scanning.h"
#include "platform.h"
#include "serial.h"

void main(void) {
    struct ScanningConfig cfg = {
        .z = 40,
        .start = {150, 120},
        .size = {500, 720},
        .res = {50, 72},
        .cal_freqs = {1, 1},
        .wait_for_sensor = false,
        .send_data = true,
        .show_lcd = false,
    };

    platform_init();

    platform_calibrate_head();
    while(!platform_calibrated());

    serial_init();
    serial_wait_for_byte();
    scanning_setup(&cfg);
    scanning_raster(X, Y);
}
