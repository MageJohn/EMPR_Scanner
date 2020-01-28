#include "lpc_types.h"
#include <stdbool.h>

// Call once before calling other functions.
void platform_init(void);

// Call to have the head go to coordinates x, y, z
//
// Parameters:
//      uint16_t x, y, z: The coordinate to move to.
void platform_head_set_coords(uint16_t x, uint16_t y, uint16_t z);

// Whether the head is at the coordinates set by platform_head_set_coords.
//
// Returns:
//      bool: true if at the coordinates, false otherwise
bool platform_head_at_coords(void);

// Start the calibration process for the head position. Once called, wait until
// platform_calibrated returns true before calling other functions on the head. 
void platform_calibrate_head(void);

// Whether the platform has finished calibrating after a call to
// platform_calibrate_head.
//
// Returns:
//      bool: true if calibrated, false otherwise. 
bool platform_calibrated(void);
