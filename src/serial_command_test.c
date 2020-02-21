#include "lpc17xx_uart.h"
#include "serial.h"
#include "leds.h"
#include "mbed/wait.h"

void remote_set_led(uint8_t *params) {
    led_clear();
    led_disp_num(params[0]);
}

void (*func_ptr_array[])(uint8_t*) = {&remote_set_led};

uint8_t remote_packet[7];

int main(void) {
    serial_init();
    UART_IntConfig(LPC_UART0, UART_INTCFG_RBR, ENABLE);
    NVIC->ISER[0] &= ~(1<<5);
    NVIC->ISER[0] |= 1<<5;
    led_setup();

    return 0;
}

void UART0_IRQHandler(void) {
    serial_read_b(remote_packet, 7);
    (*func_ptr_array[remote_packet[1]])(remote_packet + 2);
}
    
    
    
    
    
    
    

