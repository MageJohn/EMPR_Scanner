/*

  This is second part of A1, the actual file for
  drawing and making the patterns.

*/

#include <math.h>

#include "platform.h"
#include "serial.h"

#define Z_AXIS 400

// THE FUNCTIONS ASSUME THAT PLATFORM HAS BEEN INITIALIZED AND CALIBRATED
// BY USING: platform_init(); --> platform_calibrate_head(); --> while(!platform_calibrated()); !

// The stepper motors draw a square following the borders of platform
//
// Returns:
//      void
//
// Parameters:
//      void
void motor_patterns_square(void) {

    uint16_t x_coords[5] = {50, 950, 950, 50, 50};
    uint16_t y_coords[5] = {50, 50, 950, 950, 50};

    // Make a square
    int i;
    for (i = 0; i < 5; i++) {
        platform_head_set_coords_and_wait(x_coords[i], y_coords[i], Z_AXIS);
    }
}


// The stepper motors draw a circle
//
// Returns:
//      void
//
// Parameters:
//      void
void motor_patterns_circle(void) {
    double step_size = M_PI / 18;
    double radius = 400;
    int16_t center[] = {450, 450};

    double angle = 0;
    int16_t pos[2];

    // Make a circle
    for (angle = 0; angle < 2*M_PI; angle += step_size) {
        // get x and y and convert from radians to degrees

        pos[0] = (int16_t)(cos(angle) * radius) + center[0];
        pos[1] = (int16_t)(sin(angle) * radius) + center[1];

        platform_head_set_coords_and_wait(pos[0], pos[1], Z_AXIS);
    }
}


// Demonstrate the ability of platform to move up and down the z axis
//
// Returns:
//      void
//
// Parameters:
//      void
void motor_patterns_zdemo(void) {
    platform_head_set_coords_and_wait(0,0,2000);
    platform_head_set_coords_and_wait(0,0,100);
}



