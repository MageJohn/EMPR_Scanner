/*

This is second part of A1, the actual file for
drawing and making the patterns.

*/

#include "platform.h"
#include <math.h>
#include "serial.h"

#define Z_AXIS 400
#define PI 3.14159265	


// THE FUNCTIONS ASSUME THAT PLATFORM HAS BEEN INITIALIZED AND CALIBRATED
// BY USING: platform_init(); --> platform_calibrate_head(); --> while(!platform_calibrated()); !

// The stepper motors draw a square following the borders of platform
//
// Returns:
//      void
//
// Parameters:
//      void
bool draw_square(void) {
	
	uint16_t x_coords[5] = {50, 950, 950, 50, 50};
	uint16_t y_coords[5] = {50, 50, 950, 950, 50};

//	platform_init();
//	platform_calibrate_head();
//	while(!platform_calibrated());


	// Make a square
	int i;
	for (i = 0; i<5; i++) {

		platform_head_set_coords(x_coords[i],y_coords[i],Z_AXIS);
		while(!platform_head_at_coords());

	}
	
	return true;	

}


// The stepper motors draw a circle
//
// Returns:
//      void
//
// Parameters:
//      void
bool draw_circle(void) {

//	platform_init();
//	platform_calibrate_head();
//	while(!platform_calibrated());

	uint16_t angle = 0;
	uint16_t radius = 400;
	uint16_t x_center = 450;
	uint16_t y_center = 450;
	uint16_t step_size = 5;	// Decreasing this, will produce more coordinates for platform.
	uint16_t x;
	uint16_t y;
	double sin_degrees;
	double cos_degrees;
	double val = PI/180;

	// Make a circle
	while (angle < 360) {

		// get x and y and convert from radians to degrees

		sin_degrees = sin(angle*val);
		cos_degrees = cos(angle*val);		

		x = x_center + (int)(radius * cos_degrees);
		y = y_center + (int)(radius * sin_degrees);

		platform_head_set_coords(x,y,Z_AXIS);
		while(!platform_head_at_coords());

		angle += step_size;

	}

	return true;

}


// Demonstrate the ability of platform to move up and down the z axis
//
// Returns:
//      void
//
// Parameters:
//      void
bool test_vertical_axis(void) {

//	platform_init();
//	platform_calibrate_head();
//	while(!platform_calibrated());

	platform_head_set_coords(0,0,2000);
	while(!platform_head_at_coords());

	platform_head_set_coords(0,0,100);
	while(!platform_head_at_coords());

	return true;

}



