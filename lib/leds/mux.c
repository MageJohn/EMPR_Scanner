#include <string.h>
#include <stdlib.h>
#include "leds.h"

// Declarations
struct LedSource {
    uint8_t num;
    uint32_t id;
    struct LedSource *next;
};

struct LedMuxState {
    struct LedSource head;
    struct LedSource *current;
};


// Definitions
struct LedMuxState leds_mux_state = {
    .head = (struct LedSource){
        .num = 8,
        .id = 0,
        .next = NULL
    },
    .current = &leds_mux_state.head,
};


// Public functions
uint8_t* leds_mux_register_source(uint32_t id) {
    struct LedSource *current = &leds_mux_state.head;
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

    return &current->num;
}

void leds_mux_set_curr(uint32_t id) {
    struct LedSource *current = &leds_mux_state.head;
    while (current->id != id && current->next) {
        current = current->next;
    }
    if (current->id == id) {
        leds_mux_state.current = current;
    } else {
        leds_mux_state.current = &leds_mux_state.head;
    }
}

void leds_mux_tick(void) {
    if (leds_mux_state.current) {
        leds_clear();
        leds_disp_num(leds_mux_state.current->num);
    }
}
