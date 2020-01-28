#include <math.h>
#include <stdbool.h>

#include "lpc17xx_pinsel.h"
#include "lpc17xx_dac.h"

#include "mbed.h"

#define PI (3.14159265)

void mbed_dac_init(void) {
    PINSEL_CFG_Type dac_select = {
        .Funcnum = 2,
        .OpenDrain = PINSEL_PINMODE_NORMAL,
        .Pinmode = PINSEL_PINMODE_TRISTATE,
        .Pinnum = 26,
        .Portnum = 0
    };

    PINSEL_ConfigPin(&dac_select);

    DAC_Init(LPC_DAC);
}


void mbed_dac_waveform(uint32_t ampl, uint32_t freq, uint32_t time_ns) {
    uint32_t value;
    double r;

    // should add some feedback here
    if (ampl > MBED_DAC_WAVEFORM_AMPL_MAX) {
        return;
    } else if (freq > MBED_DAC_FREQ_MAX) {
        return;
    }

    uint32_t period = MBED_DAC_FREQ_MAX / freq; 

    mbed_rit_init();
    mbed_rit_set(time_ns);
    mbed_rit_state(ENABLE);

    while(!mbed_rit_get_int_status()) {
        for(r = 0; r < 2*PI; r += (PI*2)/200) {
            wait_us(period);
            value = (sin(r)+1)*((double)(ampl));
            DAC_UpdateValue(LPC_DAC, value);
        }
    }
}
