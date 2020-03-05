#include <stdio.h>
#include "platform.h"
#include "leds.h"
#include "serial.h"

#include "transmit_data_D1.h"

int x = 50;
int y = 50;
int z = 30;

void main(void){
    led_setup();
    platform_init();
    serial_init();
    
    led_mux_set_curr(HB_LED);
    
    platform_calibrate_head();
    while(!platform_calibrated());

    int i;
    for (i=0; i<10; i++){
        platform_head_set_coords(x, y, z);
        while(!platform_head_at_coords());
        x = x + 80;
        y = y + 70;
        transmit_data();
        
    }
}


