#include "ioboard.h"

// All functions assume that the IO board's I2C interface has been initialised
// with the function ioboard_i2c_init()


static uint8_t scancode8b_to_4b(uint8_t scancode8b);
static uint8_t onehot4b_to_binary2b(uint8_t onehot4b);


bool pressed_keys[NKEYS] = {[0 ... NKEYS-1] = false};


// Poll the keyboard for pressed keys
//
// Returns true if a key was pressed, false otherwise. Also set the
// pressed_keys array to contain the state of the keyboard.
//
// Returns:
//      bool: true if any key was pressed, false otherwise
//
// Parameters:
//      void
bool ioboard_keypad_poll(void) {
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
        I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
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


// Get a key from the keypad, if one is pressed
//
// If a key is pressed, put its scancode into *pressed. Returns true if
// *pressed contains a key.
//
// Returns:
//      bool: true if *pressed was filled. false otherwise
//
// Parameters:
//      uint8_t *pressed: pointer to write the pressed key to
bool ioboard_keypad_get_key(uint8_t *pressed) {
    int key;
    bool found_key = ioboard_keypad_poll();

    if (found_key) {
        for (key=0; key<NKEYS; key++) {
            if (pressed_keys[key]) {
                *pressed = key;
            }
        }
    }

    return found_key;
}


// Get a key from the keypad, but if the key is held down limit repeated reads
//
// This function operates nearly the same as ioboard_keypad_get_key, except if
// it is repeatedly polled while a key is held down, it doesn't report that a
// key is pressed every time. Instead a counter is decremented each time the
// function is called and the key is still pressed, wrapping at the value of
// rate, and a key is put into *pressed only if the value equals rate. This
// function assumes it's being called repeatedly in a loop.
//
// Returns:
//      bool: true if a key was written to *pressed
//
// Parameters:
//      uint8_t *pressed: pointer to write the pressed key to, if it's not rate
//      limited
//      uint16_t rate: limiting value for how often the key repeats when held
//      down
bool ioboard_keypad_rl_get_key(uint8_t *pressed, uint16_t rate) {
    int key;
    bool pressed_and_not_limited = false;
    static uint16_t key_repeat[16] = {[0 ... 15] = 0};

    ioboard_keypad_poll();

    for (key=0; key<NKEYS; key++) {
        if (pressed_keys[key]) {
            key_repeat[key] = (key_repeat[key] == 0) ? rate : key_repeat[key]-1;
        } else {
            key_repeat[key] = 0;
        }

        if (key_repeat[key] == rate) {
            *pressed = key;
            pressed_and_not_limited = true;
        }
    }

    return pressed_and_not_limited;
}


// Private function
//
// Converts an internal representation of the key to the external one
uint8_t scancode8b_to_4b(uint8_t scancode8b) {
    uint8_t upper_nibble = onehot4b_to_binary2b((scancode8b >> 4));
    uint8_t lower_nibble = onehot4b_to_binary2b(scancode8b);
    if (lower_nibble < 0xff && upper_nibble < 0xff) {
        return (upper_nibble << 2) | lower_nibble;
    } else {
        return 0xff;
    }
}


// Private function
//
// Helper for scancode8b_to_4b. Converts a 4 bit one hot encoding to a 2 bit
// binary number
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
