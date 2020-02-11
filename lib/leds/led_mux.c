#include <string.h>
#include <stdlib.h>
#include "leds.h"

// Declarations
struct LedMuxState {
    struct LedSource head;
    struct LedSource *current;
};


// Definitions
struct LedMuxState led_mux_state = {
    .head = (struct LedSource){
        .num = 8,
        .id = 0,
        .next = NULL
    },
    .current = NULL,
};


// Public functions
struct LedSource* led_mux_register_source(uint32_t id) {
    struct LedSource *current = &led_mux_state.head;
    while (current->id != id && current->next) {
        current = current->next;
    }

    if (current->id != id) {
        struct LedSource *new = (struct LedSource*)malloc(sizeof(struct LedSource));
        new->num = 0;
        new->id = id;
        new->next = NULL;
        current->next = new;
        current = current->next;
    }

    return current;
}

void led_mux_set_curr(uint32_t id) {
    struct LedSource *current = &led_mux_state.head;
    while (current->id != id && current->next) {
        current = current->next;
    }
    if (current->id == id) {
        led_mux_state.current = current;
    } else {
        led_mux_state.current = &led_mux_state.head;
    }
}

void led_mux_tick(void) {
    if (led_mux_state.current) {
        led_clear();
        led_disp_num(led_mux_state.current->num);
    }
}
