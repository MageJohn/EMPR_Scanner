#include "lpc_types.h"
#include <stdbool.h>

void platform_init(void);
void platform_head_set_coords(uint16_t x, uint16_t y, uint16_t z);
bool platform_head_at_coords(void);
void platform_calibrate_head(void);
bool platform_calibrated(void);
