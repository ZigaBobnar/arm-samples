#include <asf.h>
#include "runtime.h"
#include "drivers/lcd.h"

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


void lcd_handler_init(void) {
    lcd_init(&lcd);
    sprintf(lcd._lcd_string, "Inicializacija..");
    lcd_write_lcd_string(&lcd);

    delay_ms(100);

    sprintf(lcd._lcd_string, "                                ");
    lcd_write_lcd_string(&lcd);
}

void lcd_handle(void) {
    static int last_lcd_update_time = 0;

    if (last_lcd_update_time > 10000) {
        sprintf(lcd._lcd_string, "                                ");
        sprintf(lcd._lcd_string, "f = %4.1f Hz     Signal = %s",
            g_frequency,
            g_current_signal_type == SIGNAL_SINE ?
            "sinus " : "triang");

        lcd_write_lcd_string(&lcd);

        last_lcd_update_time = 0;
    }

    last_lcd_update_time++;
}
