/*------------------------------
 * FUNCTIONS
 *------------------------------*/

// THE FUNCTIONS ASSUME THAT PLATFORM HAS BEEN INITIALIZED AND CALIBRATED!

// The stepper motors draw a square following the borders of platform
//
// Returns:
//      true -- when finished
//
// Parameters:
//      void
void motor_patterns_square(void);


// The stepper motors draw a circle
//
// Returns:
//      true -- when finished
//
// Parameters:
//      void
void motor_patterns_circle(void);


// Demonstrate the ability of platform to move up and down the z axis
//
// Returns:
//      true -- when finished
//
// Parameters:
//      void
void motor_patterns_zdemo(void);
