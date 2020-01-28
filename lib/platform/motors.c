#include "mbed.h"
#include "platform.h"
#include "ioboard.h"
#include "leds.h"

#include "motors.h"

#define XYLATCH 0x3E
#define ZPENLATCH 0x3F
#define LIMITBUTTONS 0x3C

#define X_SOFT_LIMIT 950
#define Y_SOFT_LIMIT 850
#define Z_SOFT_LIMIT 1600

// The time in milliseconds between steps
#define RATE 2

// Structs
struct MotorState {
    uint8_t step;
    uint16_t coord;
    uint16_t goal_coord;
    uint16_t soft_limit;
    bool hard_limit;
    bool calibrate;
    uint8_t *led;
};


// Private function declarations
static void update_motor(struct MotorState *state);
static void update_limit_switches(void);
static void move_motors(void);


// File globals
static uint8_t steps[] = {0x09, 0x05, 0x06, 0x0a, 0};
static struct MotorState x_state = {.soft_limit = X_SOFT_LIMIT};
static struct MotorState y_state = {.soft_limit = Y_SOFT_LIMIT};
static struct MotorState z_state = {.soft_limit = Z_SOFT_LIMIT};
static uint32_t tick_start = 0;

void motors_init(void) {
    struct LedSource *x_led = led_mux_register_source(2);
    struct LedSource *y_led = led_mux_register_source(3);
    struct LedSource *z_led = led_mux_register_source(4);

    x_state.led = &x_led->num;
    y_state.led = &y_led->num;
    z_state.led = &z_led->num;
}


void motors_tick(void) {
    if (millis() - tick_start > RATE) {
        update_limit_switches();

        update_motor(&x_state);
        update_motor(&y_state);
        update_motor(&z_state);

        move_motors();

        tick_start = millis();
    }
}

void platform_head_set_coords(uint16_t x, uint16_t y, uint16_t z) {
    x_state.goal_coord = x;
    y_state.goal_coord = y;
    z_state.goal_coord = z;
}

bool platform_head_at_coords(void) {
    return (x_state.coord == x_state.goal_coord &&
            y_state.coord == y_state.goal_coord &&
            z_state.coord == z_state.goal_coord);
}

void platform_calibrate_head(void) {
    platform_head_set_coords(0, 0, 0);

    x_state.calibrate = true;
    y_state.calibrate = true;
    z_state.calibrate = true;

    x_state.coord = x_state.soft_limit;
    y_state.coord = y_state.soft_limit;
    z_state.coord = z_state.soft_limit;
}

bool platform_calibrated(void) {
    return (!x_state.calibrate &&
            !y_state.calibrate &&
            !z_state.calibrate);
}


void move_motors(void) {
    I2C_M_SETUP_Type packet = {
        .tx_length = 1,
    };
    uint8_t xy_data = steps[x_state.step] | (steps[y_state.step] << 4);
    uint8_t z_data = steps[3 - z_state.step];

    packet.sl_addr7bit = XYLATCH;
    packet.tx_data = &xy_data;
    I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);

    packet.sl_addr7bit = ZPENLATCH;
    packet.tx_data = &z_data;
    I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
}

void update_limit_switches(void) {
    uint8_t buttons;

    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = LIMITBUTTONS,
        .rx_length = 1,
        .rx_data = &buttons
    };

    I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
    x_state.hard_limit = !(bool)(buttons & 0x2);
    y_state.hard_limit = !(bool)(buttons & 0x1);
    z_state.hard_limit = !(bool)(buttons & 0x4);
}

void update_motor(struct MotorState *state) {
    int8_t direction = (state->goal_coord > state->coord) -
                       (state->goal_coord < state->coord);
    if (state->hard_limit && direction < 0) {
        if (state->calibrate) {
            state->coord = 0;
            state->calibrate = false;
        }
        state->step = 4;
        return;
    } else if (state->coord >= state->soft_limit && direction > 0) {
        state->step = 4;
        return;
    } else if (direction) {
        state->coord += direction;
        state->step = ((uint8_t)(state->step + direction)) % 4;
    } else {
        state->step = 4;
    }
    *state->led = state->step;
}
