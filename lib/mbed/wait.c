#include "mbed.h"

void wait(uint32_t time) {
    wait_ms(time * 1000);
}

void wait_ms(uint32_t time) {
    wait_us(time * 1000);
}

void wait_us(uint32_t time) {
    uint32_t start = micros();
    while(micros() - start <= time);
}
