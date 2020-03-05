#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_i2c.h"

#include "leds.h"
#include "platform.h"
#include "platform_keypad.h"
#include "time.h"


#define KEYPAD_ADDR (0x21)
#define NKEYS (16)

#define KP_INT_PIN 23


// See header for documentation of definitions in this file

/*---------------------
 * Global variables
 *---------------------
*/

uint8_t SCANCODE_CHAR_LUT[16] = {
    'D', 'C', 'B', 'A',
    '#', '9', '6', '3',
    '0', '8', '5', '2',
    '*', '7', '4', '1',
};

uint8_t SCANCODE_LCD_LUT[16] = {
    0xc4, 0xc3, 0xc2, 0xc1,
    0xa3, 0xb9, 0xb6, 0xb3,
    0xb0, 0xb8, 0xb5, 0xb2,
    0xaa, 0xb7, 0xb4, 0xb1,
};


// File local function declerations
static bool platform_keypad_poll(void);
static uint8_t scancode8b_to_4b(uint8_t scancode8b);
static uint8_t onehot4b_to_binary2b(uint8_t onehot4b);


// File local variables
static uint8_t *led;
static uint8_t *lut;
static bool pressed_keys[NKEYS] = {
    [0 ... NKEYS-1] = false
};
static I2C_M_SETUP_Type int_i2c_packet = {
    .sl_addr7bit = KEYPAD_ADDR,
};

/*---------------------
 * Public Functions
 *---------------------*/

void platform_keypad_use_lut(uint8_t *new_lut) {
    lut = new_lut;
}


/*
  Interrupt-based interface
*/

void platform_keypad_int_init(void) {
    /* Have the keypad generate an input on a keypress */
    uint8_t tx_data = 0x0F;
    int_i2c_packet = (I2C_M_SETUP_Type){
        .sl_addr7bit = KEYPAD_ADDR,
        .tx_data = &tx_data,
        .tx_length = 1,
    };
    platform_i2c_transfer_blocking(&int_i2c_packet);

    led = leds_mux_register_source(KP_LED);

    PINSEL_CFG_Type gpio_int = {
        .Funcnum = 0,
        .OpenDrain = PINSEL_PINMODE_NORMAL,
        .Pinmode = PINSEL_PINMODE_PULLUP,
        .Pinnum = KP_INT_PIN,
        .Portnum = 0,
    };
    PINSEL_ConfigPin(&gpio_int);

    // Set input
    GPIO_SetDir(0, (1 << KP_INT_PIN), 0);
    // Enable falling edge interrupt
    GPIO_IntCmd(0, (1 << KP_INT_PIN), 1);

    NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(void) {
    uint8_t reads;
    for (reads = 10; reads > 0; reads--) {
        uint32_t value = GPIO_ReadValue(0);
        if ((value & (1 << KP_INT_PIN)) == 1) {
            reads = 10;
        }

    }
    uint8_t led_time = millis();
    if ((millis() - led_time) > 1000) {
        *led ^= 1;
        led_time = millis();
    }

    // Clear the interrupt
    GPIO_ClearInt(0, (1 << KP_INT_PIN));

    // Read the PCF8574 to reset the interrupt
    uint8_t read_byte;
    int_i2c_packet = (I2C_M_SETUP_Type){
        .sl_addr7bit = KEYPAD_ADDR,
        .rx_data = &read_byte,
        .rx_length = 1,
        .tx_data = NULL,
    };

    //I2C_MasterTransferData(LPC_I2C1, &int_i2c_packet, I2C_TRANSFER_POLLING);
}


/*
  Polling-based interface
*/

bool platform_keypad_poll_key(uint8_t *pressed) {
    int key;
    bool found_key = platform_keypad_poll();

    if (found_key) {
        for (key=0; key<NKEYS; key++) {
            if (pressed_keys[key]) {
                *pressed = lut ? lut[key] : key;
            }
        }
    }

    return found_key;
}


bool platform_keypad_poll_key_rl(uint8_t *pressed, uint16_t rate) {
    int key;
    bool pressed_and_not_limited = false;
    static uint16_t key_repeat[16] = {[0 ... 15] = 0};

    platform_keypad_poll();

    for (key=0; key<NKEYS; key++) {
        if (pressed_keys[key]) {
            key_repeat[key] = (key_repeat[key] == 0) ? rate : key_repeat[key]-1;
        } else {
            key_repeat[key] = 0;
        }

        if (key_repeat[key] == rate) {
            *pressed = lut ? lut[key] : key;
            pressed_and_not_limited = true;
        }
    }

    return pressed_and_not_limited;
}


/*-------------------
 * Private functions
 *-------------------*/

/*
  Converts an internal representation of the key to the external one
*/

bool platform_keypad_poll(void) {
    uint8_t tx;
    uint8_t rx;
    uint8_t key;
    uint8_t row;
    uint8_t read_scancode;
    uint8_t test_scancode;

    bool found_key = false;

    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = KEYPAD_ADDR,
        .rx_data = &rx,
        .rx_length = 1,
        .tx_data = &tx,
        .tx_length = 1
    };

    for (tx = ~0x10; tx != 0xff; tx = ~((~tx) << 1)) {
        platform_i2c_transfer_blocking(&packet);
        read_scancode = ((~rx) & 0x0f) | (~tx);

        for (row=0x01; row <= 0x08; row <<= 1) {
            test_scancode = ~tx | row;
            key = scancode8b_to_4b(test_scancode);

            if ((read_scancode & test_scancode) == test_scancode) {
                found_key = true;
                pressed_keys[key] = true;
            } else {
                pressed_keys[key] = false;
            }
        }
    }

    return found_key;
}

uint8_t scancode8b_to_4b(uint8_t scancode8b) {
    uint8_t upper_nibble = onehot4b_to_binary2b((scancode8b >> 4));
    uint8_t lower_nibble = onehot4b_to_binary2b(scancode8b);
    if (lower_nibble < 0xff && upper_nibble < 0xff) {
        return (upper_nibble << 2) | lower_nibble;
    } else {
        return 0xff;
    }
}


/*
  Helper for scancode8b_to_4b. Converts a 4 bit one hot encoding to a 2 bit
  binary number
*/
uint8_t onehot4b_to_binary2b(uint8_t onehot4b) {
    onehot4b = onehot4b & 0x0f;
    switch (onehot4b) {
        case 0x01:
            return 0x00;
        case 0x02:
            return 0x01;
        case 0x04:
            return 0x02;
        case 0x08:
            return 0x03;
        default:
            return 0xff;
    }
}
