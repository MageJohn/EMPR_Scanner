#include "lpc_types.h"

/*
  Initialise the timer used for the rest of the functions in this library. They
  will not work without it. It is called by platform_init.
*/
void time_init(void);

/*
  Return the time in microseconds since time_init() was called.
*/
uint32_t micros(void);

/*
  Return the time in microseconds since time_init() was called.
*/
uint32_t millis(void);

/*
  Wait for a `time` seconds
*/
void wait(uint32_t time);

/*
  Wait for a `time` milliseconds
*/
void wait_ms(uint32_t time);

/*
  Wait for a `time` microseconds
*/
void wait_us(uint32_t time);

