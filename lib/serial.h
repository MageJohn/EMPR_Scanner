#include <stdbool.h>
#include "lpc_types.h"

// Serial code include file
uint32_t serial_read_nb(char *buf, uint32_t length);

uint32_t serial_read_b(char *buf, uint32_t length);

uint32_t serial_write_nb(char *buf, uint32_t length);

uint32_t serial_write_b(char *buf, uint32_t length);

bool serial_nb_write_finished(void);

void serial_init(void);

