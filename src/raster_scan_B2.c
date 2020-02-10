
#include "platform.h"
#include "raster_scan_B2.h"

// Ensure that step and end/start correlate so final step will end on end/start position
#define X_START 50
#define Y_START 50
#define Z_HEIGHT 10
#define X_END 700
#define X_STEP 10
#define Y_STEP 50
#define Y_END 500
#define X_BEGIN 60
#define MID_X 490
#define MID_Y 425

int main(void) {
    raster_scan();
    return(1);

}

// calibrates head and sets it to a specified starting location. Run flag detect first to inform x and y start!
void starting_position(uint16_t x, uint16_t y, uint16_t z) {
    platform_init();
    platform_calibrate_head();
    while(!platform_calibrated());
    platform_head_set_coords(x, y, z);
    while(!platform_head_at_coords());
}

//up to prgrammer to decide which dimension to scan. make stop_coord correspond.
//dimension chooses where to scan. 1 = x, 2 = y
bool one_dimensional_scan(uint16_t x, uint16_t y, uint16_t z, uint8_t dimension, uint16_t stop_coord, uint8_t step) {
    platform_head_set_coords(x, y, z);
    while(!platform_head_at_coords());
    bool end = false;
    while(!end) {
        switch (dimension) {
            case 1: x += step;
                    platform_head_set_coords(x, y, z);
                    while(!platform_head_at_coords());
                    //do sensor stuff here
                    if(x == stop_coord) {
                        end = true;
                    }
                    break;
            
            case 2: y += step;
                    platform_head_set_coords(x,y,z);
                    while(!platform_head_at_coords());
                    // sensor
                    if(y == stop_coord) {
                        end = true;
                    }
                    break;
            
            default: break;
        }
    }
    return true;
}

void detect_flag(int16_t mid_x, uint16_t mid_y, uint16_t z, uint16_t *x_var, uint16_t *y_var) {}

void raster_scan(uint16_t x, uint16_t y, uint16_t z, uint16_t grid_size) {
    starting_position(x, y, z);
    uint16_t x_step = (X_SOFT_LIMIT - x)/grid_size;
    uint16_t y_step = (Y_SOFT_LIMIT - y)/grid_size;
    while(true) {
        if(y == y_step) {
            break;
        }
        one_dimensional_scan(x, y, z, 1, X_SOFT_LIMIT, x_step);
        x_step *= -1;
        y += y_step;

    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
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