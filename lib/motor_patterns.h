#include "platform.h"
#include <math.h>
#include "serial.h"

/*------------------------------
 * CONSTANTS
 *------------------------------*/

#define Z_AXIS 4000
#define PI 3.14159265	

/*------------------------------
 * FUNCTIONS
 *------------------------------*/

// THE FUNCTIONS ASSUME THAT PLATFORM HAS BEEN INITIALIZED AND CALIBRATED!

// The stepper motors draw a square following the borders of platform
//
// Returns:
//      void
//
// Parameters:
//      void
void draw_square(void);


// The stepper motors draw a circle
//
// Returns:
//      void
//
// Parameters:
//      void
void draw_circle(void);


// Demonstrate the ability of platform to move up and down the z axis
//
// Returns:
//      void
//
// Parameters:
//      void
void test_vertical_axis(void);
