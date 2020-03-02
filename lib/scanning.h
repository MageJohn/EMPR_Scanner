#include <stdbool.h>
#include "lpc_types.h"

enum Axes {
    X, Y, Z
};


// each array is x, y
struct ScanningConfig {
    // The z height for all operations
    int16_t z;
    // The point the scanning starts at
    int16_t start[2];
    // The size of the area scanned
    int16_t size[2];
    // The resolution to scan in
    int16_t res[2];
    // The frequency to calibrate the axes; 0 never calibrates
    uint8_t cal_freqs[2];
    // Whether to wait for the sensor to complete an integration cycle before reading it
    bool wait_for_sensor;
    // Whether to send data down the serial.
    bool send_data;
    // Whether to write RGB data to the LCD
    bool show_lcd;
};

void scanning_setup(struct ScanningConfig *new_cfg);
void scanning_scan_axis(int16_t *coords, uint8_t axis);
void scanning_raster(uint8_t axis1, uint8_t axis2);
void scanning_two_way_raster(uint8_t axis1, uint8_t axis2);
