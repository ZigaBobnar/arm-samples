#include <asf.h>
#include <lcd.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t stevec = 0;

int main (void)
{

    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    ioport_init();
    ioport_enable_pin(PIO_PC26_IDX);
    ioport_enable_pin(PIO_PC23_IDX);
    ioport_set_pin_dir(PIO_PC26_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC23_IDX, IOPORT_DIR_INPUT);

    lcd_init();

    sprintf(lcd_string, "Initializing...                 ");
    lcd_driver();

    NVIC_EnableIRQ(ID_PIOC);
    NVIC_ClearPendingIRQ(ID_PIOC);
    NVIC_SetPriority(ID_PIOC, 1);

    pio_enable_interrupt(PIOC, PIO_PC26 | PIO_PC23);
    pio_configure_interrupt(PIOC, PIO_PC26 | PIO_PC23_IDX, PIO_IT_FALL_EDGE);



    /********************* Main loop     ***************************/
    while(1)
    {
        /*if (ioport_get_pin_level(PIO_PC26_IDX)) {
            stevec++;
        }

        if (ioport_get_pin_level(PIO_PC23_IDX)) {
            stevec--;
        }

        sprintf(lcd_string, "Stevec = %4i                    ", (int)stevec);
        lcd_driver();*/

        delay_ms(100);
    }



    while(1)
    {
    }
}


void PIOC_Handler() {
    uint32_t status = pio_get_interrupt_status(PIOC);

    if (status & PIO_PC26) {
        stevec++;
    }

    if (status & PIO_PC23) {
        stevec--;
    }

    sprintf(lcd_string, "Stevec = %4i                    ", (int)stevec);
    lcd_driver();
}

#ifdef __cplusplus
}
#endif
