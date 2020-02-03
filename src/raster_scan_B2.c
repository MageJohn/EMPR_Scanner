#include "platform.h"
#include "raster_scan_B2.h"

//TODO: find best z height 
/*
DEFINE Z_HEIGHT 
DEFINE X_LEFT
DEFINE X_RIGHT
DEFINE Y_END
*/

// Ensure that step and end/start correlate so final step will end on end/start position
#define X_START 50
#define Y_START 50
#define Z_HEIGHT 10
#define X_END 700
#define X_STEP 10
#define Y_STEP 50
#define Y_END 500
#define X_BEGIN 60

int main(void) {
    raster_scan();
    return(1);

}

void raster_scan() {
    platform_init();
    platform_calibrate_head();
    while (!platform_calibrated());
    platform_head_set_coords(X_BEGIN, Y_START, Z_HEIGHT);
    while(!platform_head_at_coords());
    uint16_t current_y = Y_START;
    uint16_t current_x = X_BEGIN;
    uint16_t step_direction = X_STEP; //reverse when end reached
    while (true) {
        if (current_y == 500) {
            break;
        }
        else if(current_x == X_START || current_x == X_END) {
            step_direction *= -1;
            current_y += Y_STEP;
            platform_head_set_coords(current_x, current_y, Z_HEIGHT);
            while(!platform_head_at_coords());
            current_x += step_direction;
            platform_head_set_coords(current_x,current_y,Z_HEIGHT);
            while(!platform_head_at_coords());
        }
        else {
            current_x += step_direction;
            platform_head_set_coords(current_x,current_y,Z_HEIGHT);
            while(!platform_head_at_coords());
        }
    }
}