#include "lpc17xx_timer.h"
#include "time.h"

void time_init(void) {
    TIM_TIMERCFG_Type timercfg = {
        .PrescaleOption = TIM_PRESCALE_USVAL,
        .PrescaleValue = 1
    };
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timercfg);
    TIM_Cmd(LPC_TIM0, ENABLE);
}

uint32_t micros(void) {
    return LPC_TIM0->TC;
}

uint32_t millis(void) {
    return micros() / 1000;
}

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
