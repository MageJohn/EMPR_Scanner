#include "lpc_types.h"
#include "serial.h"

union nibble_to_byte {
    uint8_t byte;
    struct nibbles {
        uint8_t low:4;
        uint8_t high:4;
    } nibbles;
};

union short_to_byte {
    uint16_t short_int;
    struct bytes {
        uint8_t low;
        uint8_t high;
    } bytes;
};

void main(void) {
    serial_init();
    
    //union nibble_to_byte n2b;
    //n2b.byte = 0x12;
    //uint8_t high = n2b.nibbles.high;
    //uint8_t low = n2b.nibbles.low;
    //serial_write_b((char *)&high, 1);
    //serial_write_b((char *)&low, 1);

    union short_to_byte s2b;
    s2b.short_int = 0x0110;
    uint8_t high = s2b.bytes.high;
    uint8_t low = s2b.bytes.low;
    serial_write_b((char *)&high, 1);
    serial_write_b((char *)&low, 1);
}
