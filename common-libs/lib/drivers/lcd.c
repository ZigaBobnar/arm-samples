#include "lcd.h"
#include <delay.h>

__EXTERN_C_OPEN

char lcd_buffer[33] = "                                ";
char *lcd_string = lcd_buffer;

void lcd_init(lcd_t* lcd) {
    ioport_init();

    ioport_set_pin_dir(lcd->rs, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(lcd->rw, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(lcd->enable, IOPORT_DIR_OUTPUT);

    ioport_set_pin_dir(lcd->d4, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(lcd->d5, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(lcd->d6, IOPORT_DIR_OUTPUT);
    ioport_set_pin_dir(lcd->d7, IOPORT_DIR_OUTPUT);

    delay_us(50000);

    ioport_set_pin_level(lcd->enable, 0);
    ioport_set_pin_level(lcd->rs, 0);
    ioport_set_pin_level(lcd->rw, 0);

    ioport_set_pin_level(lcd->d4, 0);
    ioport_set_pin_level(lcd->d5, 0);
    ioport_set_pin_level(lcd->d6, 0);
    ioport_set_pin_level(lcd->d7, 0);

    // TODO: Initialize LCD

    lcd_clear(lcd);

}

void lcd_write_string(lcd_t* lcd) {
    int i = 0;

    lcd_write_command(DDRAM | 0x00);

    for (; i < 16; i++) {
        lcd_send(lcd, lcd_string[i], false);
    }

    lcd_write_command(DDRAM | 0x40);

    for (; i < 32; i++) {
        lcd_send(lcd, lcd_string[i], false);
    }
}

void lcd_clear(lcd_t* lcd) {
    lcd_write_command(lcd, CLEAR_DISPLAY);

    delay_us(2000);
}

void lcd_write_command(lcd_t* lcd, uint8_t command) {
    lcd_send(lcd, command, true);
}

void lcd_send(lcd_t* lcd, uint8_t value, bool is_command) {
    ioport_set_pin_level(lcd->rs, is_command ? false : true);
    ioport_set_pin_level(lcd->rw, 0);

    ioport_set_pin_level(lcd->d7, value & (1 << 7));
    ioport_set_pin_level(lcd->d6, value & (1 << 6));
    ioport_set_pin_level(lcd->d5, value & (1 << 5));
    ioport_set_pin_level(lcd->d4, value & (1 << 4));
    
    lcd_pulse_enable(lcd);

    ioport_set_pin_level(lcd->d7, value & (1 << 3));
    ioport_set_pin_level(lcd->d6, value & (1 << 2));
    ioport_set_pin_level(lcd->d5, value & (1 << 1));
    ioport_set_pin_level(lcd->d4, value & (1 << 0));
    
    lcd_pulse_enable(lcd);
}

void lcd_pulse_enable(lcd_t* lcd) {
    ioport_set_pin_level(lcd->enable, 0);
    delay_us(1);
    ioport_set_pin_level(lcd->enable, 1);
    delay_us(1);
    ioport_set_pin_level(lcd->enable, 0);
    delay_us(100);
}


__EXTERN_C_CLOSE
