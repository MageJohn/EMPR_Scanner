#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"

#include "serial.h"

// The serial can queue a max of 16 bytes before waiting for the queue to empty again.
// The blocking functions below wait for the queue to empty and then continue, the non-blocking ones
// send or recieve a maximum of 16 bytes

uint32_t serial_read_nb(char *buf, uint32_t length) {
    return(UART_Receive((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)buf, length, NONE_BLOCKING));
}

uint32_t serial_read_b(char *buf, uint32_t length) {
  return(UART_Receive((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)buf, length, BLOCKING));
}

uint32_t serial_write_nb(char *buf, uint32_t length) {
    return(UART_Send((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)buf, length, NONE_BLOCKING));
}

uint32_t serial_write_b(char *buf, uint32_t length) {
    return(UART_Send((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)buf, length, BLOCKING));
}

bool serial_nb_write_finished(void) {
    FlagStatus status = UART_CheckBusy(LPC_UART0);
    return (status == SET) ? false : true;
}

void serial_init(void) {
    UART_CFG_Type UARTConfigStruct;  // UART Configuration structure variable
    UART_FIFO_CFG_Type UARTFIFOConfigStruct; // UART FIFO configuration Struct variable
    PINSEL_CFG_Type PinCfg;  // Pin configuration for UART

    /*
     * Initialize UART pin connect
     */
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    // USB serial first
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 2;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 3;
    PINSEL_ConfigPin(&PinCfg);

    /* Initialize UART Configuration parameter structure to default state:
     * - Baudrate = 9600bps
     * - 8 data bit
     * - 1 Stop bit
     * - None parity
     */
    UART_ConfigStructInit(&UARTConfigStruct);

    /* Initialize FIFOConfigStruct to default state:
     * - FIFO_DMAMode = DISABLE
     * - FIFO_Level = UART_FIFO_TRGLEV0
     * - FIFO_ResetRxBuf = ENABLE
     * - FIFO_ResetTxBuf = ENABLE
     * - FIFO_State = ENABLE
     */
    UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

    /*
    * Built the basic structures, lets start the devices
    * USB serial
    */

    // Initialize UART0 peripheral with given to corresponding parameter
    UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);
    // Initialize FIFO for UART0 peripheral
    UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct);
    // Enable UART Transmit
    UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);
}
