#define MBED_DAC_MAX ((1<<10)-1)
#define MBED_DAC_WAVEFORM_AMPL_MAX (MBED_DAC_MAX/2)
#define MBED_DAC_FREQ_MAX (1e6)

void mbed_dac_init(void);
void mbed_dac_waveform(uint32_t ampl, uint32_t freq, uint32_t time_ns);
