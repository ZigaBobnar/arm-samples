#include "clock_program.h"
#include <asf.h>
#include <lcd.h>

#ifdef __cplusplus
extern "C" {
#endif

// int hours = 0;
// int minutes = 0;
// int seconds = 0;
uint32_t current_time = 0;
bool clock_paused = false;
uint32_t last_time_val = 0;

void clock_task_runtime(void);
void prepare_display_text_task_runtime(void);
void display_task_runtime(void);
void buttons_task_runtime(void);

void clock_program_init(void) {
    // Init IO
    ioport_init();

    ioport_enable_pin(PIO_PC26_IDX);
    ioport_enable_pin(PIO_PC25_IDX);
    ioport_enable_pin(PIO_PC24_IDX);
    ioport_enable_pin(PIO_PC23_IDX);

    ioport_set_pin_dir(PIO_PC26_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC25_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC24_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC23_IDX, IOPORT_DIR_INPUT);

    // Init lcd
    lcd_init();
    sprintf(lcd_string, "Inicializacija...              ");
    lcd_driver();

    // Init timer counter
    sysclk_enable_peripheral_clock(ID_TC1);

    tc_init(TC0, 1, TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_WAVE);
    tc_start(TC0, 1);
    last_time_val = tc_read_cv(TC0, 1);
}

rtos_task_t clock_task = {
    .function = clock_task_runtime, // pika je pomembna!
};

rtos_task_t prepare_display_text_task = {
    .function = prepare_display_text_task_runtime,
};

rtos_task_t display_task = {
    .function = display_task_runtime,
};

rtos_task_t buttons_task = {
    .function = buttons_task_runtime,
};

rtos_task_t null_task = {
    .function = NULL,
};

rtos_task_t *rtos_clock_program_tasks[] = {
    &clock_task,
    &prepare_display_text_task,
    &display_task,
    &buttons_task,
    &null_task,
};


void clock_task_runtime(void) {
    if (!clock_paused) {
        const uint32_t time_unit = 1000 * (CLOCKS_PER_SEC * 32);

        uint32_t new_time_val = tc_read_cv(TC0, 1);
        while (new_time_val - last_time_val >= time_unit) {
            last_time_val += time_unit;

            current_time++;
        }
    } else {
        last_time_val = tc_read_cv(TC0, 1);
    }

    /*while (seconds >= 60) {
        minutes++;
        seconds -= 60;
    }

    while (minutes >= 60) {
        hours++;
        minutes -= 60;
    }

    if (hours >= 24) {
        hours = 0;
    }*/
}

void prepare_display_text_task_runtime(void) {
    static const uint32_t day_seconds = 24 * 60 * 60;
    int hours, minutes, seconds;

    if (current_time >= day_seconds) {
        current_time = 0;
    }

    hours = current_time % (3600);
    minutes = (current_time - (hours * 3600)) % 60;
    seconds = (current_time - (hours * 3600) - (seconds * 60));

    sprintf(lcd_string, "Ura: %02i:%02i:%02i                   ", hours, minutes, seconds);
}

void display_task_runtime(void) {
    lcd_driver();
}

void buttons_task_runtime(void) {
    static uint32_t old_buttons_state = 0;

    uint32_t buttons_state =
        (!ioport_get_pin_level(PIO_PC26_IDX) << 3) |
        (!ioport_get_pin_level(PIO_PC25_IDX) << 2) |
        (!ioport_get_pin_level(PIO_PC24_IDX) << 1) |
        (!ioport_get_pin_level(PIO_PC23_IDX) << 0);

    uint32_t rising_edge = ~old_buttons_state & buttons_state;
    uint32_t falling_edge = old_buttons_state & ~buttons_state;

    if ((falling_edge >> 3) & 1) {
        current_time += 3600;
        // hours++;
    }

    if ((falling_edge >> 2) & 1) {
        current_time += 60;
        // minutes++;
    }

    if ((rising_edge >> 1) & 1) {
        clock_paused = true;
    }

    if ((falling_edge >> 1) & 1) {
        clock_paused = false;
    }

    if ((falling_edge >> 0) & 1) {
        current_time = 0;
        // hours = 0;
        // minutes = 0;
        // seconds = 0;
    }

    old_buttons_state = buttons_state;
}

#ifdef __cplusplus
}
#endif
