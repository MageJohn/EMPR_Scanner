#include "platform.h"
#include "mbed.h"
#include <stdlib.h>
#include "serial.h"

void scan(void);

#define X_STEPS 10
#define Y_STEPS 5

uint64_t data[Y_STEPS];

void main(void){
    scan();
}

void scan(void){
    int x = X_SOFT_LIMIT;
    int y = Y_SOFT_LIMIT;
    platform_init();
    platform_calibrate_head();
    while(!platform_calibrated());
    platform_head_set_coords(X_SOFT_LIMIT-1,Y_SOFT_LIMIT-1,30);
    while(!platform_head_at_coords());
    //GET RGB VALS
    int ystep = Y_SOFT_LIMIT / Y_STEPS;
    int xstep = X_SOFT_LIMIT / X_STEPS;
    int i;
    int z;
    serial_init();
    for(z=0; z < X_STEPS/2; z++) {
        for(i=0; i < Y_STEPS; y++){
            y = y - ystep;
            platform_head_set_coords(x,y,30);
            while(!platform_head_at_coords());
            //GET RGB DATA
            platform_sensor_get_data(&data[i]);
        }
        x = x - xstep;
        platform_head_set_coords(x,y,30);
        while(!platform_head_at_coords());
        // TRANSFER RGB DATA
        serial_write_b((char *)data, sizeof(uint64_t) * Y_STEPS);
        wait(1); //REMOVE AFTER TEST
        for(i=0; i < Y_STEPS; y++){
            y = y + ystep;
            platform_head_set_coords(x,y,30);
            while(!platform_head_at_coords());
            //GET RGB VALS
            platform_sensor_get_data(&data[i]);
        }
        x = x - xstep;
        platform_head_set_coords(x,y,30);
        while(!platform_head_at_coords());
        
    }
}


/*void init_send_data(void) {
    serial_init();
    uint16_t buffer1 = ;
    uint16_t buffer2 = ;
    serial_write_b((char *)&buffer1, sizeof(uint16_t));
    serial_write_b((char *)&buffer2, sizeof(uint16_t));
}*/