#include <asf.h>
#include "runtime.h"

#define ACTIVE_ADC_CHANNEL ADC_CHANNEL_7

void adc_handler_init(void) {
    sysclk_enable_peripheral_clock(ID_ADC);
    adc_init(ADC, sysclk_get_main_hz(), ID_ADC, ADC_STARTUP_TIME_4);
    adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
    adc_set_resolution(ADC, ADC_12_BITS);
    adc_configure_trigger(ADC, ADC_TRIG_SW, 0);
    adc_enable_channel(ADC, ACTIVE_ADC_CHANNEL);
}

uint32_t adc_read_value(void) {
    adc_start(ADC);

    uint32_t adc_status;
    do {
        adc_status = adc_get_status(ADC);
    } while (!(adc_status & ADC_ISR_DRDY));

    return adc_get_channel_value(ADC, ACTIVE_ADC_CHANNEL);
}

void adc_handle(void) {
    static uint32_t last_frequency_val_read_time = 0;

    if (last_frequency_val_read_time > 1000) {
        uint32_t adc_value = adc_read_value();
        float new_frequency = ((float)adc_value / 4095) * 29 + 1;

        if (new_frequency != g_frequency) {
            g_frequency = new_frequency;
            dac_update_frequency();
        }

        last_frequency_val_read_time = 0;
    }

    last_frequency_val_read_time++;
}
