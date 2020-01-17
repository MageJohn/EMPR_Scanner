#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "ioboard/i2c.h"

// Initialise the i2c bus to which the ioboard peripherals attached
//
// Returns:
//      void
//
// Parameters:
//      void
void ioboard_i2c_init(void) {
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
}
