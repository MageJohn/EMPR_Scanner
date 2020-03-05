#include <stdbool.h>
#include "lpc_types.h"

#define SERIAL_MAX_STR_LEN 256

// Serial code include file
uint32_t serial_read_nb(char *buf, uint32_t length);

uint32_t serial_read_b(char *buf, uint32_t length);

uint32_t serial_write_nb(char *buf, uint32_t length);

uint32_t serial_write_b(char *buf, uint32_t length);

uint32_t serial_printf(char const *format, ...);

bool serial_nb_write_finished(void);

void serial_wait_for_byte(void);

void serial_init(void);

