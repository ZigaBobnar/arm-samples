#include <asf.h>
#include <time.h>
#include <math.h>
#include "runtime.h"

#define DAC_SAMPLE_COUNT 100
int sine_wave[DAC_SAMPLE_COUNT];
int triangular_wave[DAC_SAMPLE_COUNT];

void dac_handler_init(void) {
    sysclk_enable_peripheral_clock(ID_TC0);

    sysclk_enable_peripheral_clock(ID_DACC);
    dacc_reset(DACC);
    dacc_set_transfer_mode(DACC, DACC_MR_WORD_HALF); // 16 bit transfer
    dacc_set_timing(DACC, 0x08, 0, 0x10);
    dacc_set_channel_selection(DACC, 1);
    dacc_enable_channel(DACC, 1);

    NVIC_EnableIRQ(ID_TC0);
    tc_init(TC0, 0, TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC);

    dac_prepare_wave_tables();

    dac_update_frequency();
}

void dac_prepare_wave_tables(void) {
    for (int i = 0; i < DAC_SAMPLE_COUNT; i++) {
        sine_wave[i] = 4095 * (sin(3.14 / 2 * (DAC_SAMPLE_COUNT - 1 + 2*i) / (DAC_SAMPLE_COUNT - 1)) + 1) / 2;
        triangular_wave[i] = 4095 * i / (DAC_SAMPLE_COUNT - 1);
    }
}

void dac_handle(void) {
    uint32_t dacc_status = dacc_get_interrupt_status(DACC);
    if ((dacc_status & DACC_ISR_TXRDY) == DACC_ISR_TXRDY) {
        if (g_current_signal_value_index >= DAC_SAMPLE_COUNT) {
            g_signal_direction = -1;
            g_current_signal_value_index = DAC_SAMPLE_COUNT - 2;
        }

        int signal_value;
        if (g_current_signal_type == SIGNAL_SINE) {
            signal_value = sine_wave[g_current_signal_value_index];
        } else {
            signal_value = triangular_wave[g_current_signal_value_index];
        }

        dacc_write_conversion_data(DACC, signal_value);

        g_current_signal_value_index += g_signal_direction;

        if (g_current_signal_value_index <= 0) {
            g_signal_direction = 1;
            g_current_signal_value_index = 1;
        }
    }
}

void dac_update_frequency(void) {
    tc_disable_interrupt(TC0, 0, TC_IER_CPCS);
    tc_stop(TC0, 0);

    float T_sample_ms = 1000.0 / (g_frequency * 2*(DAC_SAMPLE_COUNT - 1));
    tc_write_rc(TC0, 0, (uint32_t)(T_sample_ms * CLOCKS_PER_SEC * 32));

    tc_start(TC0, 0);
    tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
    // TODO: Clear counter value.
}
