#include <asf.h>
#include <lcd.h>

#ifdef __cplusplus
extern "C" {
#endif

void adc_setup(void);
uint32_t adc_read(void);

int main (void)
{

    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    delay_init();

    lcd_init();
    sprintf(lcd_string, "Inicializacija...              ");
    lcd_driver();

    adc_setup();


    /********************* Main loop     ***************************/
    while(1)
    {
        uint32_t value = adc_read();


        sprintf(lcd_string, "AD = %4i                       ", (int)value);
        lcd_driver();

        delay_ms(400);
    }



    while(1)
    {
    }
}


void adc_setup()
{
    sysclk_enable_peripheral_clock(ID_ADC);

    adc_init(ADC, sysclk_get_main_hz(), ID_ADC, ADC_STARTUP_TIME_4);
    adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
    adc_set_resolution(ADC, ADC_12_BITS);
    adc_configure_trigger(ADC, ADC_TRIG_SW, 0);
    adc_enable_channel(ADC, ADC_CHANNEL_7);
}

uint32_t adc_read(void)
{
    adc_start(ADC);

    uint32_t status;
    do {
        status = adc_get_status(ADC);
    }
    while ((status & ADC_ISR_DRDY) != ADC_ISR_DRDY);

    return adc_get_channel_value(ADC, ADC_CHANNEL_7);
}

#ifdef __cplusplus
}
#endif
