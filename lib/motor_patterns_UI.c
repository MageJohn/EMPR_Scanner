/*
This is part one of A1, this file combines all patters mentioned in A1
and lets the user choose between them.
*/

#include "UI.h"
#include "motor_patterns.h"

struct UI_elem square_elem = {
    .text = "square",
    .callback = &motor_patterns_square,
};

struct UI_elem circle_elem = {
    .text = "circle",
    .callback = &motor_patterns_circle,
};

struct UI_elem zdemo_elem = {
    .text = "zdemo",
    .callback = &motor_patterns_zdemo,
};


void select_test(void) {
    UI_build_option_menu(NULL, &square_elem);
    UI_build_option_menu(&square_elem, &circle_elem);
    UI_build_option_menu(&circle_elem, &zdemo_elem);

    UI_run(&square_elem);
}
