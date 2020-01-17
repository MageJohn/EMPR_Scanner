#include "rit.h"
#include "wait.h"

void wait(uint32_t time) {
    wait_ms(time * 1000);
}

void wait_ms(uint32_t time) {
    wait_us(time * 1000);
}

void wait_us(uint32_t time) {
    mbed_rit_init();
    mbed_rit_set(time);
    mbed_rit_state(ENABLE);

    while (!mbed_rit_get_int_status()) {
        continue;
    }
}
