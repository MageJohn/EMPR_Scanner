#include "lpc17xx_rit.h"
#include "lpc17xx_clkpwr.h"

#include "mbed.h"

void mbed_rit_init(void) {
    RIT_Init(LPC_RIT);
}

void mbed_rit_deinit(void) {
    RIT_DeInit(LPC_RIT);
}

// note, based on a clock rate of 25,000,000 (what the mbed runs at for this
// project) and the maximum number stored in 32 bits, approximately 171,798,691
// nanoseconds (171 seconds) is the maximum time
void mbed_rit_set(uint32_t time_us) {
    uint32_t clock_rate, cmp_value;

    clock_rate = CLKPWR_GetPCLK(CLKPWR_PCLKSEL_RIT);
    /*
     * clock_rate in Hz (ticks per second)
     * time in us (microseconds)
     * clock_rate/1e6 is ticks per microsecond
     * cmp_value is the number of ticks until the interrupt fires
     */
    cmp_value = (clock_rate / 1e6) * time_us;
    LPC_RIT->RICOMPVAL = cmp_value;
    LPC_RIT->RICTRL |= (1<<1);
}

void mbed_rit_state(FunctionalState newstate) {
    RIT_Cmd(LPC_RIT, newstate);
}

IntStatus mbed_rit_get_int_status() {
    return RIT_GetIntStatus(LPC_RIT);
}
