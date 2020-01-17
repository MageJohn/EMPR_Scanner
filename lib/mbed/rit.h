#include "lpc_types.h"

void mbed_rit_init(void);
void mbed_rit_deinit(void);
void mbed_rit_set(uint32_t time_ns);
void mbed_rit_state(FunctionalState newstate);
IntStatus mbed_rit_get_int_status();
