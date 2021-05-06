#include <asf.h>
#include "runtime.h"

#define BUTTON_T1_POS 1
#define BUTTON_T2_POS 0
#define BUTTON_T1_MASK (1 << BUTTON_T1_POS)
#define BUTTON_T2_MASK (1 << BUTTON_T2_POS)

void buttons_handler_init(void) {
    ioport_init();

    ioport_enable_pin(PIO_PC26_IDX);
    ioport_enable_pin(PIO_PC25_IDX);

    ioport_set_pin_dir(PIO_PC26_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC25_IDX, IOPORT_DIR_INPUT);
}

uint8_t buttons_get_state(void) {
    return (
        !ioport_get_pin_level(PIO_PC26_IDX) << BUTTON_T1_POS |
        !ioport_get_pin_level(PIO_PC25_IDX) << BUTTON_T2_POS);
}

void buttons_handle(void) {
    static uint8_t previous_state = 0x00;

    uint8_t new_state = buttons_get_state();
    uint8_t falling_edges = previous_state & ~new_state;
    
    if (falling_edges & BUTTON_T1_MASK) {
        if (g_current_signal_type != SIGNAL_SINE) {
            g_current_signal_type = SIGNAL_SINE;
            g_current_signal_value_index = 0;
            g_signal_direction = 1;
        }
    } else if (falling_edges & BUTTON_T2_MASK) {
        if (g_current_signal_type != SIGNAL_TRIANGULAR) {
            g_current_signal_type = SIGNAL_TRIANGULAR;
            g_current_signal_value_index = 0;
            g_signal_direction = 1;
        }
    }

    previous_state = new_state;
}
