/*

This file combines parts A1, A2, A3 and B1 into a single user interface
file, from which the previously mentioned parts can be accessed
and demonstrated.

*/

#include "UI.h"
#include "platform.h"
#include "platform_lcd.h"
#include "motor_patterns_UI.h"
#include "manual_move_UI.h"
#include "detect_edges.h"
#include "time.h"

static void run_manual_ui(void);

struct UI_elem patt_elem = {
    .text = "patt test",
    .callback = &select_test,
};

struct UI_elem edge_elem = {
    .text = "edge detect",
    .callback = &detect_edges,
};

struct UI_elem manual_elem = {
    .text = "move/scan",
    .callback = &run_manual_ui,
};

void main(void) {
    platform_init();
    platform_lcd_init();
    platform_lcd_clear_display();

    platform_calibrate_head();
    while(!platform_calibrated());

    detect_edges();

    // Build main menu
    UI_build_option_menu(NULL, &patt_elem);
    UI_build_option_menu(&patt_elem, &edge_elem);
    UI_build_option_menu(&edge_elem, &manual_elem);

    // Go into UI
    UI_run(&patt_elem);
}

void run_manual_ui(void) {
    manual_ui(UI_SHOW_COORDS, NULL);
    wait_ms(50);
}
