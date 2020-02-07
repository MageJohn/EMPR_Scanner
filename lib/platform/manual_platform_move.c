#include "ioboard.h"
#include "platform.h"

// All functions assumes previous I2c init. with ioboard_i2c_init()
// and previous platform_init() call and calibration

uint16_t x_axis = 0;
uint16_t y_axis = 0;
uint16_t z_axis = 0;


void move_axis_up(uint16_t axis) {

	axis += 50;
	platform_head_set_coords(x_axis, y_axis, z_axis);

}

void move_axis_down(uint16_t axis) {

	axis -= 50;
	platform_head_set_coords(x_axis, y_axis, z_axis);

}