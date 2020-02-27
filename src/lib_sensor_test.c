#include <stdio.h>
#include "platform.h"
#include "leds.h"
#include "serial.h"

#define OUT_BUF_MAX 256

void main(void) {
    union ColourData data;
    uint8_t out_buf[OUT_BUF_MAX];
    int out_len;

    platform_init();
    leds_init();
    serial_init();

    leds_mux_set_curr(HB_LED);

    out_len = snprintf((char *)out_buf, OUT_BUF_MAX, "start");
    serial_write_b((char *)out_buf, (uint32_t)out_len);

    while (1) {
        platform_sensor_get_data(data.combined);
        serial_write_b((char *)data.low_high, 8);
    }
}
