#include <asf.h>
#include <math.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SAMPLES 10
int sine_wave[SAMPLES];
int triangular_wave[SAMPLES];

enum { SIGNAL_SINE, SIGNAL_TRIANGULAR } signal = SIGNAL_SINE;
float frequency = 2;
uint8_t signal_value_index = 0;
int8_t signal_direction = 1;

int main (void)
{
    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    sysclk_enable_peripheral_clock(ID_TC0);

    //delay_init();

    //ioport_init();

    sysclk_enable_peripheral_clock(ID_DACC);
    dacc_reset(DACC);
    dacc_set_transfer_mode(DACC, DACC_MR_WORD_HALF); // 16 bit transfer
    dacc_set_timing(DACC, 0x08, 0, 0x10);
    dacc_set_channel_selection(DACC, 1);
    dacc_enable_channel(DACC, 1);

    for (int i = 0; i < SAMPLES; i++) {
        sine_wave[i] = 4095 * (sin(3.14 / 2 * (SAMPLES - 1 + 2*i) / (SAMPLES - 1)) + 1) / 2;
        triangular_wave[i] = 4095 * i / (SAMPLES - 1);
    }

    NVIC_EnableIRQ(ID_TC0);
    tc_init(TC0, 0, TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC);
    tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
    float T_ms = 1000.0 / frequency;
    tc_write_rc(TC0, 0, (uint32_t)(T_ms * CLOCKS_PER_SEC * 32 / SAMPLES / 2));
    tc_start(TC0, 0);

    /********************* Main loop     ***************************/
    while(1)
    {

    }

    while (1) {}
}

void TC0_Handler(void) {
    uint32_t dacc_status = dacc_get_interrupt_status(DACC);
    if ((dacc_status & DACC_ISR_TXRDY) == DACC_ISR_TXRDY) {
        if (signal_value_index >= SAMPLES) {
            signal_direction = -1;
            signal_value_index = SAMPLES - 2;
        }

        int signal_value = signal == SIGNAL_SINE ?
            sine_wave[signal_value_index] :
            triangular_wave[signal_value_index];

        dacc_write_conversion_data(DACC, signal_value);

        signal_value_index += signal_direction;

        if (signal_value_index <= 0) {
            signal_direction = 1;
            signal_value_index = 1;
        }
    }

    tc_get_status(TC0, 0);
}

#ifdef __cplusplus
}
#endif
