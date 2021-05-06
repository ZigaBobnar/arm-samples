#include <asf.h>
#include <math.h>
#include <time.h>
#include <math.h>
#include "runtime.h"

#ifdef __cplusplus
extern "C" {
#endif

signal_type g_current_signal_type = SIGNAL_SINE;
float g_frequency = 2.0;
int g_current_signal_value_index = 0;
int g_signal_direction = 1;

int main (void)
{
    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    buttons_handler_init();
    adc_handler_init();
    dac_handler_init();
    lcd_handler_init();

    /********************* Main loop     ***************************/
    while(1)
    {
        buttons_handle();
        adc_handle();
        lcd_handle();
    }

    while (1) {}
}

void TC0_Handler(void) {
    dac_handle();
    tc_get_status(TC0, 0);
}

#ifdef __cplusplus
}
#endif
