#include "lpc17xx_timer.h"
#include "mbed.h"

void timer_init(void) {
    TIM_TIMERCFG_Type timercfg = {
        .PrescaleOption = TIM_PRESCALE_USVAL,
        .PrescaleValue = 1000
    };
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timercfg);
    TIM_Cmd(LPC_TIM0, ENABLE);
}

uint32_t millis(void) {
    return LPC_TIM0->TC;
}
