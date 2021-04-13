#include <asf.h>

#ifdef __cplusplus
extern "C" {
#endif

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

    PIOB->PIO_PER = 1 << 27;
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
    PIOC->PIO_SODR = 1 << 30;

    bool stanje = false;

    /********************* Main loop     ***************************/
    while(1)
    {
        if (stanje) {
            PIOB->PIO_SODR = 1 << 27;
            PIOA->PIO_CODR = 1 << 21;
            PIOC->PIO_CODR = 1 << 30;

        } else {
            PIOB->PIO_CODR = 1 << 27;
            PIOA->PIO_SODR = 1 << 21;
            PIOC->PIO_SODR = 1 << 30;

        }

        stanje = !stanje;

        cakaj(4800000);
    }



    // Preventative loop
    while(1)
    {
    }
}

void cakaj(uint32_t n) {
    for (int i = 0; i < n; ++i) {}
}

#ifdef __cplusplus
}
#endif
