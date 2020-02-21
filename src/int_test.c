#include <stdbool.h>
#include "lpc_types.h"
#include "lpc17xx_timer.h"
#include "leds.h"

#define PRIQ_LEN 7

struct PriNode {
    uint32_t value;
    void *data;
};

struct PriNode callq[PRIQ_LEN + 1];
uint32_t callq_end = 1;
// The first cell will be used to hold the index of the end of the queue

Status call_deferred(void (*callback)(void), uint32_t time) {
    NVIC_DisableIRQ(TIMER1_IRQn);

    if (callq_end >= PRIQ_LEN + 1) {
        return ERROR;
    }

    callq[callq_end] = (struct PriNode) {time + LPC_TIM1->TC, callback};

    uint32_t node = callq_end;
    uint32_t parent = node / 2;
    struct PriNode tmp;

    while (node > 1 && callq[parent].value > callq[node].value) {
        tmp = callq[parent];
        callq[parent] = callq[node];
        callq[node] = tmp;

        node = parent;
        parent = (node - 1) / 2;
    }

    callq_end++;
    NVIC_EnableIRQ(TIMER1_IRQn);

    return SUCCESS;
}

void TIMER1_IRQHandler(void) {
    if (callq_end <= 1) {
        callq_end = 1;
        return;
    }
    struct PriNode popped = callq[1];

    callq[1] = callq[callq_end - 1];

    uint32_t node = ;
    uint32_t child = 

    while (callq[0])
}

void main(void) {
    led_setup();

    led_disp_num(1);

    TIM_TIMERCFG_Type timercfg = {
        .PrescaleOption = TIM_PRESCALE_USVAL,
        .PrescaleValue = 1000
    };

    TIM_MATCHCFG_Type matchcfg = {
        .MatchChannel = 0,
        .IntOnMatch = ENABLE,
        .ResetOnMatch = DISABLE,
        .StopOnMatch = DISABLE,
    };

    TIM_Cmd(LPC_TIM1, DISABLE);
    TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &timercfg);
    TIM_ConfigMatch(LPC_TIM1, &matchcfg);
    TIM_UpdateMatchValue(LPC_TIM1, 0, 1000);
    TIM_ResetCounter(LPC_TIM1);
    NVIC_EnableIRQ(TIMER1_IRQn);
    TIM_Cmd(LPC_TIM1, ENABLE);
}

