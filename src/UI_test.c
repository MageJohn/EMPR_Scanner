#include "platform.h"
#include "platform_lcd.h"
#include "UI.h"
#include "leds.h"

static void incr_leds(void);
static void decr_leds(void);
static void zero_leds(void);

struct UI_elem incr_elem;
struct UI_elem decr_elem;
struct UI_elem zero_elem;

uint8_t *leds;

struct UI_elem incr_elem = {
    .text = "incr leds",
    .callback = &incr_leds,
};

struct UI_elem decr_elem = {
    .text = "decr leds",
    .callback = &decr_leds,
};

struct UI_elem zero_elem = {
    .text = "zero leds",
    .callback = &zero_leds,
};

void main(void) {
    platform_init();
    platform_lcd_init();
    platform_lcd_clear_display();

    leds = leds_mux_register_source(1);
    leds_mux_set_curr(1);

    UI_build_sequence(NULL, &incr_elem);
    UI_build_sequence(&incr_elem, &decr_elem);
    UI_build_sequence(&decr_elem, &zero_elem);

    UI_run(&incr_elem);
}

void incr_leds(void) {
    *leds += 1;
}

void decr_leds(void) {
    *leds -= 1;
}

void zero_leds(void) {
    *leds = 0;
}
