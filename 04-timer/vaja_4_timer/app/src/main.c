#include <asf.h>
#include <lcd.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

// Prototypes
char read_buttons(void);
void process_inputs(void);

// Global variables
char duty_cycle = 50;
char frequency = 0;
uint32_t t_on = 1;
uint32_t t_off = 1;
uint32_t tc_cycle_start = 0;
bool is_on = false;
char previous_buttons_state = 0x00;

int main (void)
{
    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    sysclk_enable_peripheral_clock(ID_TC0);

    delay_init();

    ioport_init();
    // LED
    ioport_enable_pin(PIO_PC22_IDX);
    // Tipke
    ioport_enable_pin(PIO_PC26_IDX);
    ioport_enable_pin(PIO_PC25_IDX);
    ioport_enable_pin(PIO_PC24_IDX);
    ioport_enable_pin(PIO_PC23_IDX);

    ioport_set_pin_dir(PIO_PC22_IDX, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(PIO_PC26_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC25_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC24_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC23_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_level(PIO_PC22_IDX, IOPORT_PIN_LEVEL_LOW);

    lcd_init();
    sprintf(lcd_string, "Initializing...                 ");
    lcd_driver();

    tc_init(TC0, 0, TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_WAVE);
    // tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
    tc_start(TC0, 0);
    tc_cycle_start = tc_read_cv(TC0, 0);

    /********************* Main loop     ***************************/
    while(1)
    {
        char old_duty_cycle = duty_cycle;
        char old_frequency = frequency;

        process_inputs();

        if (old_duty_cycle != duty_cycle || old_frequency != frequency) {
            sprintf(lcd_string, "DUTY = %2i       FREQ = %2i       ", (int)duty_cycle, (int)frequency);
            lcd_driver();

            float T_ms_percent = 1000.0 / frequency / 100;
            t_on = (int)(T_ms_percent * duty_cycle * CLOCKS_PER_SEC * 32);
            t_off = (int)(T_ms_percent * (100 - duty_cycle) * CLOCKS_PER_SEC * 32);
        }

        uint32_t current_time = tc_read_cv(TC0, 0);

        if (is_on && current_time - tc_cycle_start > t_on) {
            is_on = false;
            ioport_set_pin_level(PIO_PC22_IDX, IOPORT_PIN_LEVEL_LOW);
        }

        if (!is_on && current_time - tc_cycle_start > t_off + t_on) {
            is_on = true;
            ioport_set_pin_level(PIO_PC22_IDX, IOPORT_PIN_LEVEL_HIGH);
            tc_cycle_start = current_time;
        }

        //ioport_set_pin_level(PIO_PC22_IDX, IOPORT_PIN_LEVEL_HIGH);
        //delay_ms(t_on);
        //ioport_set_pin_level(PIO_PC22_IDX, IOPORT_PIN_LEVEL_LOW);
        //delay_ms(t_off);
    }



    while(1)
    {
    }
}

char read_buttons()
{
    bool t1 = ioport_get_pin_level(PIO_PC26_IDX);
    bool t2 = ioport_get_pin_level(PIO_PC25_IDX);
    bool t3 = ioport_get_pin_level(PIO_PC24_IDX);
    bool t4 = ioport_get_pin_level(PIO_PC23_IDX);

    bool run1 = previous_buttons_state >> 3 & 1 && !t1;
    bool run2 = previous_buttons_state >> 2 & 1 && !t2;
    bool run3 = previous_buttons_state >> 1 & 1 && !t3;
    bool run4 = previous_buttons_state >> 0 & 1 && !t4;

    previous_buttons_state = (t1 << 3) | (t2 << 2) | (t3 << 1) | (t4 << 0);

    return (run1 << 3) | (run2 << 2) | (run3 << 1) | (run4 << 0);
}

void process_inputs()
{
    char buttons = read_buttons();
    if (buttons >> 3 & 1) {
        frequency -= 1;
    } else if (buttons >> 2 & 1) {
        frequency += 1;
    }

    if (buttons >> 1 & 1) {
        duty_cycle -= 5;
    } else if (buttons >> 0 & 1) {
        duty_cycle += 5;
    }

    if (frequency > 20) {
        frequency = 20;
    } else if (frequency < 1) {
        frequency = 1;
    }

    if (duty_cycle > 95) {
        duty_cycle = 95;
    } else if (duty_cycle < 5) {
        duty_cycle = 5;
    }
}

#ifdef __cplusplus
}
#endif
