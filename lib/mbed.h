#include "lpc_types.h"

void timer_init(void);
uint32_t micros(void);
uint32_t millis(void);

void mbed_rit_init(void);
void mbed_rit_deinit(void);
void mbed_rit_set(uint32_t time_ns);
void mbed_rit_state(FunctionalState newstate);
IntStatus mbed_rit_get_int_status();

#define MBED_DAC_MAX ((1<<10)-1)
#define MBED_DAC_WAVEFORM_AMPL_MAX (MBED_DAC_MAX/2)
#define MBED_DAC_FREQ_MAX (1e6)

void mbed_dac_init(void);
void mbed_dac_waveform(uint32_t ampl, uint32_t freq, uint32_t time_ns);

void wait(uint32_t time);
void wait_ms(uint32_t time);
void wait_us(uint32_t time);

