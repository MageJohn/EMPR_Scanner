#include "platform.h"
#include "serial.h"
#include "transmit_data_D1.h"


bool transmit_data(void){
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t z = 0;
    uint64_t rgb_data[1];
    platform_head_get_coords(&x, &y, &z);
    platform_sensor_get_data((uint16_t *)&rgb_data);
    serial_write_b((char *)(int16_t[]){x, y, z}, sizeof(int16_t) * 3);
    serial_write_b((char *)rgb_data, sizeof(uint64_t));
    return true;
}