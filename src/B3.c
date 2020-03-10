#include "scanning.h"
#include "platform.h"
#include "UI.h"

struct ScanningConfig cfg;

struct UI_elem clear_elem;
struct UI_elem red_elem;
struct UI_elem green_elem;
struct UI_elem blue_elem;

static void goto_clear(void);
static void goto_red(void);
static void goto_green(void);
static void goto_blue(void);

struct UI_elem clear_elem = {
    .text = "goto clear",
    .callback = &goto_clear,
};

struct UI_elem red_elem = {
    .text = "goto red",
    .callback = &goto_red,
};

struct UI_elem green_elem = {
    .text = "goto green",
    .callback = &goto_green,
};

struct UI_elem blue_elem = {
    .text = "goto blue",
    .callback = &goto_blue,
};

void main(void) {
    platform_init();
    serial_init();
    platform_lcd_init();
    platform_lcd_clear_display();

    cfg = (struct ScanningConfig){
        .z = 200,
        .start = {100, 600},
        .size = {600, 1},
        .res = {120, 1},
        .cal_freqs = {0, 0},
        .wait_for_sensor = false,
        .send_data = false,
        .show_lcd = false,
        .highest_vals = {0, 0, 0, 0},
        .location_highest = {{0, 0, 0},{0, 0, 0},{0, 0, 0},{0, 0, 0}}
    };

    platform_calibrate_head();
    while(!platform_calibrated());

    scanning_setup(&cfg);

    scanning_raster(X, Y);

    UI_build_option_menu(NULL, &clear_elem);
    UI_build_option_menu(&clear_elem, &red_elem);
    UI_build_option_menu(&red_elem, &green_elem);
    UI_build_option_menu(&green_elem, &blue_elem);

    UI_run(&clear_elem);
}

static void goto_clear(void) {
    int16_t *pos = cfg.location_highest[0];
    platform_head_set_coords(pos[0], pos[1], pos[2]);}

void goto_red(void) {
    int16_t *pos = cfg.location_highest[1];
    platform_head_set_coords(pos[0], pos[1], pos[2]);
}

void goto_green(void) {
    int16_t *pos = cfg.location_highest[2];
    platform_head_set_coords(pos[0], pos[1], pos[2]);
}

void goto_blue(void) {
    int16_t *pos = cfg.location_highest[3];
    platform_head_set_coords(pos[0], pos[1], pos[2]);
}