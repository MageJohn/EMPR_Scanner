#include <stdlib.h>
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"

#include "queue.h"

#include "platform.h"
#include "platform_i2c.h"

static struct Queue userq;
static struct Queue systq;

/*-------------------
 * Public functions
 * ------------------*/

Status platform_i2c_schedule_transfer(I2C_M_SETUP_Type *packet) {
    return push(&userq, (void *)packet);
}

Status platform_i2c_transfer_blocking(I2C_M_SETUP_Type *packet) {
    enum I2C_Status status = platform_i2c_schedule_transfer(packet);

    if (status == ERROR) {
        return status;
    }

}

Status platform_i2c_write(uint8_t addr, uint8_t *data, uint32_t length) {
    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = addr,
        .tx_data = data,
        .tx_length = length,
    };
    return platform_i2c_transfer_blocking(&packet);
}

Status platform_i2c_read(uint8_t addr, uint8_t *buffer, uint32_t length) {
    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = addr,
        .rx_data = buffer,
        .rx_length = length,
    };
    return platform_i2c_transfer_blocking(&packet);
}

/*---------------------------
 * Package private functions
 * --------------------------*/

void i2c_init(void) {
    PINSEL_CFG_Type i2c1_select;

    i2c1_select = (PINSEL_CFG_Type){
        .Funcnum = 3,
        .OpenDrain = PINSEL_PINMODE_NORMAL,
        .Pinmode = PINSEL_PINMODE_PULLUP,
        .Pinnum = 0,
        .Portnum = 0
    };

    PINSEL_ConfigPin(&i2c1_select);
    i2c1_select.Pinnum = 1;
    PINSEL_ConfigPin(&i2c1_select);

    I2C_Init(LPC_I2C1, 100000);
    I2C_Cmd(LPC_I2C1, ENABLE);

    transfer_ll.head = NULL;
    transfer_ll.tail = NULL;
}

void i2c_tick(void) {
    struct Transfer *current = transfer_ll.head;

    if (current) {
        while (current->next != NULL && current->status != RUNNING) {
            current = current->next;
        }

        if (current->status == RUNNING) {
            Status status = I2C_MasterTransferData(LPC_I2C1,
                                                    current->packet,
                                                    I2C_TRANSFER_POLLING);

            current->status = status ? FINISHED : FAILED;
        }
    }
}
