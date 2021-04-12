#include <asf.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <delay.h>

void cakaj(uint32_t n);

int main (void)
{

    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    // Led L: PIOB-27
    // Led TX: PIOA-21
    // Led RX: PIOC-30

    delay_init();
    ioport_init();

    ioport_enable_pin(PIO_PB27_IDX);
    ioport_enable_pin(PIO_PA21_IDX);
    ioport_enable_pin(PIO_PC30_IDX);

    ioport_set_pin_dir(PIO_PB27_IDX, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(PIO_PA21_IDX, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(PIO_PC30_IDX, IOPORT_DIR_OUTPUT);

    ioport_set_pin_level(PIO_PB27_IDX, IOPORT_PIN_LEVEL_LOW);
    ioport_set_pin_level(PIO_PA21_IDX, IOPORT_PIN_LEVEL_HIGH);
    ioport_set_pin_level(PIO_PC30_IDX, IOPORT_PIN_LEVEL_HIGH);

    /*PIOB->PIO_PER = 1 << 27;
    PIOB->PIO_OER = 1 << 27;
    PIOB->PIO_OWER = 1 << 27;
    PIOB->PIO_SODR = 1 << 27;

    PIOA->PIO_PER = 1 << 21;
    PIOA->PIO_OER = 1 << 21;
    PIOA->PIO_OWER = 1 << 21;
    PIOA->PIO_SODR = 1 << 21;

    PIOC->PIO_PER = 1 << 30;
    PIOC->PIO_OER = 1 << 30;
    PIOC->PIO_OWER = 1 << 30;
    PIOC->PIO_SODR = 1 << 30;*/

    bool stanje = false;

    /********************* Main loop     ***************************/
    while(1)
    {
        if (stanje) {
            ioport_set_pin_level(PIO_PB27_IDX, IOPORT_PIN_LEVEL_HIGH);
            ioport_set_pin_level(PIO_PA21_IDX, IOPORT_PIN_LEVEL_LOW);
            ioport_set_pin_level(PIO_PC30_IDX, IOPORT_PIN_LEVEL_LOW);

            /*PIOB->PIO_SODR = 1 << 27;
            PIOA->PIO_CODR = 1 << 21;
            PIOC->PIO_CODR = 1 << 30;*/
        } else {
            ioport_set_pin_level(PIO_PB27_IDX, IOPORT_PIN_LEVEL_LOW);
            ioport_set_pin_level(PIO_PA21_IDX, IOPORT_PIN_LEVEL_HIGH);
            ioport_set_pin_level(PIO_PC30_IDX, IOPORT_PIN_LEVEL_HIGH);

            /*PIOB->PIO_CODR = 1 << 27;
            PIOA->PIO_SODR = 1 << 21;
            PIOC->PIO_SODR = 1 << 30;*/
        }

        stanje = !stanje;

        cakaj(1000);
    }



    // Preventative loop
    while(1)
    {
    }
}

void cakaj(uint32_t n) {
    //for (uint32_t i = 0; i < n; ++i) {}

    delay_ms(n);
}

#ifdef __cplusplus
}
#endif
