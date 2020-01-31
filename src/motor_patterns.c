#include "platform.h"
#include <math.h>

#define Z_AXIS 20


// For testing purposes
void main(void) {

	// run code
	square();
	

}

void square(void) {
	
	uint16_t x_coords[5] = {50, 950, 950, 50, 50};
	uint16_t y_coords[5] = {50, 50, 950, 950, 50};

	// Set to starting point and initialize
	platform_init();
	platform_calibrate_head();
	while(!platform_calibrated());


	// Make a square
	int i;
	for (i = 0; i<5; i++) {

		platform_head_set_coords(x_coords[i],y_coords[i],Z_AXIS);
		while(!platform_head_at_coords());

	}
		


}


// TODO: Not working :c
void circle(void) {

	uint8_t theta = 0;
	uint16_t radius = 50;
	uint16_t x_center = 250;
	uint16_t y_center = 250;
	uint16_t step_size = 1;
	uint16_t x;
	uint16_t y;

	
	platform_init();
	platform_calibrate_head();
	while(!platform_calibrated());

	while (theta <= 360) {
		
		x = x_center + (radius * cos(theta));
		y = y_center + (radius * sin(theta));

		platform_head_set_coords(x,y,Z_AXIS);
		while(!platform_head_at_coords());

		theta += step_size;

	}

}


