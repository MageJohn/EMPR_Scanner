#include "scanning.h"
#include "platform.h"
#include "platform_lcd.h"
#include "time.h"
#include <stdlib.h>
#include "serial.h"
#include "UI.h"

// Boolean for sending data
bool send = true;
char transfer_elem_false[] = "To pc: false";
char transfer_elem_true[] = "To pc: true";
char data_to_screen[20];

void start_raster_scan(void);
void enable_transfer(void);

struct UI_elem transfer_elem = {
    .text = transfer_elem_true,
    .callback = &enable_transfer,
};

struct UI_elem raster_elem = {
    .text = "Start",
    .callback = &start_raster_scan,
};

void main(void) {
    platform_init();
    serial_init();

    platform_calibrate_head();
    while(!platform_calibrated());

    UI_build_option_menu(NULL, &transfer_elem);
    UI_build_option_menu(&transfer_elem, &raster_elem);

    UI_run(&transfer_elem);

}


void enable_transfer(void) {

    send = !send;
    if (send) {
        transfer_elem.text = transfer_elem_true;
    } else {
        transfer_elem.text = transfer_elem_false;
    }
}


void start_raster_scan(void) {

    struct ScanningConfig cfg = {
        .z = 100,
        .start = {0, 0},
        .size = {980, 850},
        .res = {100, 100},
        .cal_freqs = {0, 1},
        .wait_for_sensor = false,
        .send_data = send,
        .show_lcd = true,
    };

    if (send) {

        platform_lcd_clear_display();
        strcpy(data_to_screen, "Waiting for");
        platform_lcd_write_ascii(data_to_screen, 0);
        sprintf(data_to_screen, "serial.");
        platform_lcd_write_ascii(data_to_screen, 64);

        serial_wait_for_byte();

    }

    scanning_setup(&cfg);
    scanning_raster(X, Y);

}

