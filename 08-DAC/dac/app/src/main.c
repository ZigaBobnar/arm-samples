#include <asf.h>
#include <math.h>
#include <time.h>
#include <math.h>

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

uint8_t old_buttons_state = 0x00;

void tablify_waves(void);
void update_frequency(void);
uint32_t adc_read_value(void);

int main (void)
{
    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    sysclk_enable_peripheral_clock(ID_TC0);

    //delay_init();

    ioport_init();
    ioport_enable_pin(PIO_PC26_IDX);
    ioport_enable_pin(PIO_PC25_IDX);

    ioport_set_pin_dir(PIO_PC26_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC25_IDX, IOPORT_DIR_INPUT);

    sysclk_enable_peripheral_clock(ID_ADC);
    adc_init(ADC, sysclk_get_main_hz(), ID_ADC, ADC_STARTUP_TIME_4);
    adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
    adc_set_resolution(ADC, ADC_12_BITS);
    adc_configure_trigger(ADC, ADC_TRIG_SW, 0);
    adc_enable_channel(ADC, ADC_CHANNEL_7);


    sysclk_enable_peripheral_clock(ID_DACC);
    dacc_reset(DACC);
    dacc_set_transfer_mode(DACC, DACC_MR_WORD_HALF); // 16 bit transfer
    dacc_set_timing(DACC, 0x08, 0, 0x10);
    dacc_set_channel_selection(DACC, 1);
    dacc_enable_channel(DACC, 1);

    NVIC_EnableIRQ(ID_TC0);
    tc_init(TC0, 0, TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC);

    tablify_waves();
    update_frequency();

    /********************* Main loop     ***************************/
    while(1)
    {
        uint8_t new_buttons_state =
            !ioport_get_pin_level(PIO_PC26_IDX) << 1 |
            !ioport_get_pin_level(PIO_PC25_IDX) << 0;

        uint8_t falling_edge_buttons = old_buttons_state & ~new_buttons_state;

        if ((falling_edge_buttons >> 1) & 1) {
            signal = SIGNAL_SINE;
        } else if ((falling_edge_buttons >> 0) & 1) {
            signal = SIGNAL_TRIANGULAR;
        }

        static uint32_t last_frequency_read_time = 0;
        if (last_frequency_read_time > 1000) {
            uint32_t adc_value = adc_read_value();
            float new_frequency = 1 + adc_value;

            if (new_frequency != frequency) {
                frequency = new_frequency;
                update_frequency();
            }

            last_frequency_read_time = 0;
        } else {
            last_frequency_read_time++;
        }

        old_buttons_state = new_buttons_state;
    }

    while (1) {}
}

void tablify_waves(void) {
    for (int i = 0; i < SAMPLES; i++) {
        sine_wave[i] = 4095 * (sin(3.14 / 2 * (SAMPLES - 1 + 2*i) / (SAMPLES - 1)) + 1) / 2;
        triangular_wave[i] = 4095 * i / (SAMPLES - 1);
    }
}

void update_frequency(void) {
    tc_disable_interrupt(TC0, 0, TC_IER_CPCS);
    tc_stop(TC0, 0);

    float T_sample_ms = 1000.0 / (frequency * 2*(SAMPLES - 1));
    tc_write_rc(TC0, 0, (uint32_t)(T_sample_ms * CLOCKS_PER_SEC * 32));

    tc_start(TC0, 0);
    tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
    // TODO: Clear counter value.
}

uint32_t adc_read_value(void) {
    adc_start(ADC);

    uint32_t status;
    do {
        status = adc_get_status(ADC);
    } while (!(status & ADC_ISR_DRDY));

    return adc_get_channel_value(ADC, ADC_CHANNEL_7);
}

void TC0_Handler(void) {
    uint32_t dacc_status = dacc_get_interrupt_status(DACC);
    if ((dacc_status & DACC_ISR_TXRDY) == DACC_ISR_TXRDY) {
        if (signal_value_index >= SAMPLES) {
            signal_direction = -1;
            signal_value_index = SAMPLES - 2;
        }

        int signal_value;
        if (signal == SIGNAL_SINE) {
            signal_value = sine_wave[signal_value_index];
        } else {
            signal_value = triangular_wave[signal_value_index];
        }

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
