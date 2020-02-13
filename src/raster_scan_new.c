#include "platform.h"
#include "leds.h"
#include "serial.h"
#include "time.h"

#define Z (200)

const uint16_t X_START = 280;
const uint16_t Y_START = 400;
const uint16_t X_STOP = X_SOFT_LIMIT;
const uint16_t Y_STOP = Y_SOFT_LIMIT;
const uint16_t X_RES = 50;
const uint16_t Y_RES = 25;


void main(void) {
    union ColourData data[Y_RES];

    uint16_t x_scale = (X_STOP - X_START)/X_RES;
    uint16_t y_scale = (Y_STOP - Y_START)/Y_RES;

    uint16_t data_sent;
    uint16_t *data_ready;

    uint16_t y = Y_START;

    platform_init();
    platform_calibrate_head();

    led_setup();
    led_mux_set_curr(HB_LED);

    serial_init();

    serial_write_nb((char *)&X_RES, 2);
    serial_write_nb((char *)&Y_RES, 2);

    platform_scanner_config(data, Y_RES, y_scale);

    while(!platform_calibrated());

    for (uint16_t x = X_START; x < X_STOP; x += x_scale) {
        platform_head_set_coords(x, y, Z);
        while(!platform_head_at_coords());

        y = (y == Y_STOP) ? Y_START : Y_STOP;

        data_sent = 0;
        data_ready = platform_scanner_scan_to(x, y, Z);

        while(*data_ready < Y_RES) {
            // Send any data while waiting
            if (*data_ready > data_sent) {
                serial_write_b((char *)data, 8);
                data_sent++;
            }
        }
        // Finish sending data
        while (*data_ready > data_sent) {
            serial_write_b((char *)data, 8);
            data_sent++;
        }
    }
}
