#include <string.h>
#include <stdio.h>

#include "platform_lcd.h"
#include "serial.h"
#include "time.h"
#include "platform.h"

uint16_t platform_x_edge;
uint16_t platform_y_edge;

uint8_t red;
uint8_t blue;
uint8_t green;

uint16_t x;
uint16_t y;
uint16_t z;

uint64_t rgb_buffer[1];

bool detection_completed = false;
bool x_flag = true;
bool y_flag = false;

char data_to_screen[30];

int main(void) {

    serial_init();
    platform_init();
    platform_lcd_init();

	platform_calibrate_head();
	while(!platform_calibrated());

    platform_head_set_coords(300,300,0);
    while(!platform_head_at_coords());

    detect_edges();

    return 1;

}


void detect_edges(void) {

    while(!detection_completed) {

        get_rgb_values();

        platform_head_get_coords(&x, &y, &z);

        if (x_flag && red > 2 && blue > 2 && green > 2) {

            platform_head_set_coords(2000, y, z);

        } else if (x_flag && red < 2 && blue < 2 && green < 2) {

            platform_x_edge = x;
            x_flag = false;
            y_flag = true;
            platform_head_set_coords(300,300,0);
            while(!platform_head_at_coords());

        } else if (y_flag && red > 2 && blue > 2 && green > 2) {

            platform_head_set_coords(x, 0, z);

        } else if (y_flag && red < 2 && blue < 2 && green < 2) {

            platform_y_edge = Y_SOFT_LIMIT;
            y_flag = false;
            detection_completed = true;

        }

    }

    platform_lcd_clear_display();
    strcpy(data_to_screen, "Edges:");
    platform_lcd_write_ascii(data_to_screen, 0);

    strcpy(data_to_screen, "X: 980  Y: 860");
    platform_lcd_write_ascii(data_to_screen, 64);

    platform_calibrate_head();
	while(!platform_calibrated());

}


void get_rgb_values() {

    platform_sensor_get_data(&rgb_buffer);

    uint16_t *p = (uint16_t *)&rgb_buffer;

    // Convert from 16-bit raw data to 8-bit RGB representation
    red = ((float)p[1]/65536)*255;
    green = ((float)p[2]/65536)*255;
    blue = ((float)p[3]/65536)*255;

}


int get_x_edge(void) {
    return X_SOFT_LIMIT;
}

int get_y_edge(void) {
    return Y_SOFT_LIMIT;
}