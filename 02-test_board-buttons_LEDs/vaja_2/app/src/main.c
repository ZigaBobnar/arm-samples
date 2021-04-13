#include <asf.h>

#ifdef __cplusplus
extern "C" {
#endif

int get_button_state(void);

int main (void)
{

    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    // Tipke: Pin 4 -> 7 => C26,C25,C24,C23
    // LED: Pin 2 -> 5 => C22,C21,C29,D7
    ioport_init();

    ioport_enable_pin(PIO_PC26_IDX);
    ioport_enable_pin(PIO_PC25_IDX);
    ioport_enable_pin(PIO_PC24_IDX);
    ioport_enable_pin(PIO_PC23_IDX);

    ioport_enable_pin(PIO_PC22_IDX);
    ioport_enable_pin(PIO_PC21_IDX);
    ioport_enable_pin(PIO_PC29_IDX);
    ioport_enable_pin(PIO_PD7_IDX);

    ioport_set_pin_dir(PIO_PC26_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC25_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC24_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC23_IDX, IOPORT_DIR_INPUT);

    ioport_set_pin_dir(PIO_PC22_IDX, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(PIO_PC21_IDX, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(PIO_PC29_IDX, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(PIO_PD7_IDX, IOPORT_DIR_OUTPUT);

    /*pio_set_debounce_filter(PIOC, PIO_PC26, .5);
    pio_set_debounce_filter(PIOC, PIO_PC25, .5);
    pio_set_debounce_filter(PIOC, PIO_PC24, .5);
    pio_set_debounce_filter(PIOC, PIO_PC23, .5);*/


    /********************* Main loop     ***************************/
    while(1)
    {

        int buttonState = get_button_state();

        /*
        // Directly write button state to pins:
        ioport_set_pin_level(PIO_PC22_IDX, buttonState >> 3 & 1);
        ioport_set_pin_level(PIO_PC21_IDX, buttonState >> 2 & 1);
        ioport_set_pin_level(PIO_PC29_IDX, buttonState >> 1 & 1);
        ioport_set_pin_level(PIO_PD7_IDX, buttonState >> 0 & 1);*/

        // Toggle button states (if buttonState has bit set to 1 => toggle that pin)
        if (buttonState >> 3 & 1) {
            ioport_set_pin_level(
                PIO_PC22_IDX,
                !ioport_get_pin_level(PIO_PC22_IDX)
            );
        }
        if (buttonState >> 2 & 1) {
            ioport_set_pin_level(
                PIO_PC21_IDX,
                !ioport_get_pin_level(PIO_PC21_IDX)
            );
        }
        if (buttonState >> 1 & 1) {
            ioport_set_pin_level(
                PIO_PC29_IDX,
                !ioport_get_pin_level(PIO_PC29_IDX)
            );
        }
        if (buttonState >> 0 & 1) {
            ioport_set_pin_level(
                PIO_PD7_IDX,
                !ioport_get_pin_level(PIO_PD7_IDX)
            );
        }
    }



    while(1)
    {
    }
}

// This stores previous state of pins
int oldState = 0x00;

int get_button_state(void) {
    // Read all input pins (values are negated)
    bool a = !ioport_get_pin_level(PIO_PC26_IDX);
    bool b = !ioport_get_pin_level(PIO_PC25_IDX);
    bool c = !ioport_get_pin_level(PIO_PC24_IDX);
    bool d = !ioport_get_pin_level(PIO_PC23_IDX);

    // Converts bit values into an integer
    int newState = a << 3 | b << 2 | c << 1 | d << 0;

    // Magick...
    bool ta=0, tb=0, tc=0, td=0;
    // Get bit value from the old state
    // If previous state == 1 AND current state == 0 => We are on falling edge. So toggle we must
    ta = (oldState >> 3 & 1) && !a;
    tb = (oldState >> 2 & 1) && !b;
    tc = (oldState >> 1 & 1) && !c;
    td = (oldState >> 0 & 1) && !d;

    // Convert differences into an integer so main loop can toggle them
    int difference = ta << 3 | tb << 2 | tc << 1 | td << 0;

    // Store new state into old state for next time this loops runs
    oldState = newState;

    return difference;
}



#ifdef __cplusplus
}
#endif
