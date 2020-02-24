#include "platform.h"
#include "mbed.h"
#include <stdlib.h>
#include "serial.h"

uint16_t data[100] = {}; //Scan certain points in the image and append the rgb vals to the array

char detect(uint16_t pixels[100]);

void main(void) {
    detect(data);

}


char detect(uint16_t pixels[100]) {
    int i;
    for(i = 0; i < sizeof(data); i++)
}