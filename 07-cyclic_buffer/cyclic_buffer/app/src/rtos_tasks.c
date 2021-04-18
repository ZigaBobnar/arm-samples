#include "rtos_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif

lcd_t lcd = {
    .rs = PIO_PC12_IDX,     // Due pin 51, 	SAM3X8E pin PC12
    .rw	= PIO_PC14_IDX,     // Due pin 49,	SAM3X8E pin PC14
    .enable = PIO_PC16_IDX, // Due pin 47	SAM3X8E pin PC16

    .d4 = PIO_PC13_IDX,     // Due pin 50,	SAM3X8E pin PC13
    .d5 = PIO_PC15_IDX,     // Due pin 48,	SAM3X8E pin PC15
    .d6 = PIO_PC17_IDX,     // Due pin 46,	SAM3X8E pin PC17
    .d7 = PIO_PC19_IDX,     // Due pin 44,	SAM3X8E pin PC19

    .__lcd_buffer = "                                ",
};

uint8_t button_presses_buffer[KEYS_FIFO_SIZE];
fifo_t button_presses_fifo = { 0, 0, KEYS_FIFO_SIZE - 1, button_presses_buffer };

uint8_t old_buttons_state = 0x00;

char queue[10] = "          ";

uint32_t last_timer_value = 0;

void rtos_tasks_init(void) {
    // Init buttons and LED
    ioport_init();
    ioport_enable_pin(PIO_PC26_IDX);
    ioport_enable_pin(PIO_PC25_IDX);
    ioport_enable_pin(PIO_PC24_IDX);
    ioport_enable_pin(PIO_PC23_IDX);

    ioport_enable_pin(PIO_PC22_IDX);

    ioport_set_pin_dir(PIO_PC26_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC25_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC24_IDX, IOPORT_DIR_INPUT);
    ioport_set_pin_dir(PIO_PC23_IDX, IOPORT_DIR_INPUT);

    ioport_set_pin_dir(PIO_PC22_IDX, IOPORT_DIR_OUTPUT);

    // Init lcd
    lcd_init(&lcd);
    sprintf(lcd._lcd_string, "Inicializacija...");
    lcd_write_lcd_string(&lcd);

    // Init timer counter
    sysclk_enable_peripheral_clock(ID_TC1);

    tc_init(TC0, 1, TC_CMR_TCCLKS_TIMER_CLOCK3 | TC_CMR_WAVE);
    tc_start(TC0, 1);
    last_timer_value = tc_read_cv(TC0, 1);
}


void buttons_task_runtime(void);
void led_driver_task_runtime(void);
void lcd_driver_task_runtime(void);

uint8_t keys_buff[KEYS_FIFO_SIZE];
fifo_t keys_fifo = { 0, 0, KEYS_FIFO_SIZE, keys_buff };

rtos_task_t buttons_task = {
    .function = buttons_task_runtime,
};

rtos_task_t led_driver_task = {
    .function = led_driver_task_runtime,
};

rtos_task_t lcd_driver_task = {
    .function = lcd_driver_task_runtime,
};

rtos_task_t null_task = {
    .function = NULL,
};

rtos_task_t* rtos_tasklist[] = {
    &buttons_task,
    &led_driver_task,
    &lcd_driver_task,
    &null_task,
};

void buttons_task_runtime(void) {
    uint8_t new_buttons_state =
            (!ioport_get_pin_level(PIO_PC26_IDX) << 3) |
            (!ioport_get_pin_level(PIO_PC25_IDX) << 2) |
            (!ioport_get_pin_level(PIO_PC24_IDX) << 1) |
            (!ioport_get_pin_level(PIO_PC23_IDX) << 0);

    uint8_t rising_edge = ~old_buttons_state & new_buttons_state;
    uint8_t falling_edge = old_buttons_state & ~new_buttons_state;

    if ((falling_edge >> 3) & 1) {
        fifo_write_single(&button_presses_fifo, '1');
    }
    if ((falling_edge >> 2) & 1) {
        fifo_write_single(&button_presses_fifo, '2');
    }
    if ((falling_edge >> 1) & 1) {
        fifo_write_single(&button_presses_fifo, '3');
    }
    if ((falling_edge >> 0) & 1) {
        fifo_write_single(&button_presses_fifo, '4');
    }

    old_buttons_state = new_buttons_state;
}

void led_driver_task_runtime(void) {
    //delay_us(100);
}

void lcd_driver_task_runtime(void) {
    uint8_t pending_buttons[KEYS_FIFO_SIZE];
    uint8_t actual_pending = 0;

    actual_pending = fifo_peek(&button_presses_fifo, pending_buttons, KEYS_FIFO_SIZE - 1);

    for (int i = 0; i < 16; ++i) {
        queue[i] = (i < actual_pending) ? pending_buttons[i] : ' ';
    }

    sprintf(lcd._lcd_string, queue);
    lcd_write_lcd_string(&lcd);
}

#ifdef __cplusplus
}
#endif
