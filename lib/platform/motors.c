#include "time.h"
#include "platform.h"
#include "leds.h"

#include "motors.h"

#define XYLATCH 0x3E
#define ZPENLATCH 0x3F
#define LIMITBUTTONS 0x3C

// The time in milliseconds between steps
#define DEFAULT_INTERVAL 2

// Structs
struct MotorState {
    uint8_t step;
    int16_t coord;
    int16_t goal_coord;
    int16_t soft_limit;
    bool hard_limit;
    bool calibrate;
    uint8_t *led;
};


// File private functions
static void update_motor(struct MotorState *state);
static void update_limit_switches(void);
static void move_motors(void);


// File globals
static uint8_t steps[] = {0x09, 0x05, 0x06, 0x0a, 0};
static struct MotorState x_state = {.soft_limit = X_SOFT_LIMIT};
static struct MotorState y_state = {.soft_limit = Y_SOFT_LIMIT};
static struct MotorState z_state = {.soft_limit = Z_SOFT_LIMIT};
static uint32_t tick_start = 0;
static uint16_t interval = DEFAULT_INTERVAL; 
static uint8_t *limit_switch_led;

/* ---------------------
 * Public functons
 * ---------------------*/

void platform_head_set_coords(int16_t x, int16_t y, int16_t z) {
    x_state.goal_coord = x > X_SOFT_LIMIT ? X_SOFT_LIMIT : x;
    y_state.goal_coord = y > Y_SOFT_LIMIT ? Y_SOFT_LIMIT : y;
    z_state.goal_coord = z > Z_SOFT_LIMIT ? Z_SOFT_LIMIT : z;
}

bool platform_head_at_coords(void) {
    return (x_state.coord == x_state.goal_coord &&
            y_state.coord == y_state.goal_coord &&
            z_state.coord == z_state.goal_coord);
}

void platform_head_move_to(int16_t x, int16_t y, int16_t z) {
    platform_head_set_coords(x, y, z);
    while (!platform_head_at_coords());
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

void platform_motor_update_interval(uint16_t new_interval) {
    interval = new_interval;
}

void platform_head_get_position(int16_t *x, int16_t *y, int16_t *z) {
    *x = x_state.coord;
    *y = y_state.coord;
    *z = z_state.coord;
}

/*----------------------------
 * Library private functions
 * ---------------------------*/

void motors_init(void) {
    x_state.led = leds_mux_register_source(X_LED);
    y_state.led = leds_mux_register_source(Y_LED);
    z_state.led = leds_mux_register_source(Z_LED);
    limit_switch_led = leds_mux_register_source(LIM_SWITCH_LED);
}


void motors_tick(void) {
    if (millis() - tick_start > interval) {
        update_limit_switches();

        update_motor(&x_state);
        update_motor(&y_state);
        update_motor(&z_state);

        move_motors();

        tick_start = millis();
    }
}


/*--------------------------
 * File private functins
 * -------------------------*/

void move_motors(void) {
    I2C_M_SETUP_Type packet = {
        .tx_length = 1,
    };
    uint8_t xy_data = steps[x_state.step] | (steps[y_state.step] << 4);
    uint8_t z_data = steps[(3 - z_state.step) % 5];
    static uint8_t old_xy;
    static uint8_t old_z;

    if (xy_data != old_xy) {
        packet.sl_addr7bit = XYLATCH;
        packet.tx_data = &xy_data;
        I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
    }

    if (z_data != old_z) {
        packet.sl_addr7bit = ZPENLATCH;
        packet.tx_data = &z_data;
        I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
    }

    old_xy = xy_data;
    old_z = z_data;
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
    *limit_switch_led = x_state.hard_limit | y_state.hard_limit << 1 | z_state.hard_limit << 2;
}

void update_motor(struct MotorState *state) {
    int8_t direction = (state->goal_coord > state->coord) -
                       (state->goal_coord < state->coord);

    if (state->hard_limit && direction < 0) {
        if (state->calibrate) {
            state->calibrate = false;
        }
        state->coord = 0;
        state->goal_coord = 0;
        state->step = 4;
    } else if (state->coord >= state->soft_limit && direction > 0) {
        state->step = 4;
    } else if (direction) {
        if (!state->calibrate) {
            state->coord += direction;
        }
        state->step = ((uint8_t)(state->step + direction)) % 4;
    } else {
        state->step = 4;
    }

    *state->led = state->step != 4;
}
