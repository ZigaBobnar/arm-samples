#include <asf.h>
#include "drivers/lcd.h"

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


int main (void)
{
    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    lcd_init(&lcd);

    sprintf(lcd._lcd_string, "test");
    lcd_write_lcd_string(&lcd);

    delay_ms(2000);

    lcd_command_set_ddram_address(&lcd, 0x00);
    lcd_write_string_at_cursor(&lcd, " Hello", 6);
    lcd_command_set_ddram_address(&lcd, 0x40);
    lcd_write_string_at_cursor(&lcd, " world!", 7);


    /********************* Main loop     ***************************/
    while(1)
    {
        lcd_command_display_on_off(&lcd, true, false, false);
        delay_ms(500);

        lcd_command_display_on_off(&lcd, false, false, false);
        delay_ms(500);
    }
}


#ifdef __cplusplus
}
#endif
