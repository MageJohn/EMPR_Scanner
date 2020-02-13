#include "lpc_types.h"

// Serial code include file
uint32_t serial_read_nb(char *buf, uint32_t length);

uint32_t serial_read_b(char *buf, uint32_t length);

uint32_t serial_write_nb(char *buf, uint32_t length);

uint32_t serial_write_b(char *buf, uint32_t length);

void serial_init(void);

