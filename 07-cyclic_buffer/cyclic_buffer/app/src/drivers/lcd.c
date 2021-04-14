/*
 * drivers/lcd.c
 *
 *  Created on: Sep 23, 2016
 *      Author: vaje
 */



#ifdef __cplusplus
extern "C" {
#endif

#include "drivers/lcd.h"

char lcd_buff[33] = "                                ";
char *lcd_string = lcd_buff;


// Get LCD cursor position
uint8_t lcd_get_cursor(void){
  uint8_t position;
/*  ioport_set_port_dir(IOPORT_PIOC,\
                      _LCD_D4_PIO | _LCD_D5_PIO | _LCD_D6_PIO | _LCD_D7_PIO, IOPORT_DIR_INPUT);
                      */

  ioport_set_pin_dir(_PIN_LCD_D4, IOPORT_DIR_INPUT);
  ioport_set_pin_dir(_PIN_LCD_D5, IOPORT_DIR_INPUT);
  ioport_set_pin_dir(_PIN_LCD_D6, IOPORT_DIR_INPUT);
  ioport_set_pin_dir(_PIN_LCD_D7, IOPORT_DIR_INPUT);


  ioport_set_pin_level(_PIN_LCD_RS, 0);
  ioport_set_pin_level(_PIN_LCD_RW, 1);
  delay_us(1);
  ioport_set_pin_level(_PIN_LCD_E, 1);
  delay_us(1);


  position = 0 | (ioport_get_pin_level(_PIN_LCD_D6) << 6) | \
    ioport_get_pin_level(_PIN_LCD_D5) << 5 | \
    ioport_get_pin_level(_PIN_LCD_D4)<<4;


  ioport_set_pin_level(_PIN_LCD_E, 0);
  delay_us(1);
  ioport_set_pin_level(_PIN_LCD_E, 1);
  delay_us(1);



  position |= (ioport_get_pin_level(_PIN_LCD_D7) << 3) | \
    ioport_get_pin_level(_PIN_LCD_D6) << 2 | \
    ioport_get_pin_level(_PIN_LCD_D5)<<1 | \
    ioport_get_pin_level(_PIN_LCD_D5)<<0;

  ioport_set_pin_level(_PIN_LCD_E, 0);
  /*ioport_set_port_dir(IOPORT_PIOC,\
                      _LCD_D4_PIO | _LCD_D5_PIO | _LCD_D6_PIO | _LCD_D7_PIO, IOPORT_DIR_OUTPUT);*/



  ioport_set_pin_dir(_PIN_LCD_D4, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(_PIN_LCD_D5, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(_PIN_LCD_D6, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(_PIN_LCD_D7, IOPORT_DIR_OUTPUT);

  return position;

}

// Wait for busy state of LCD to end
void busy(void){
  uint32_t busy_flag;

/*  ioport_set_port_dir(IOPORT_PIOC,\
                      _LCD_D4_PIO | _LCD_D5_PIO | _LCD_D6_PIO | _LCD_D7_PIO, IOPORT_DIR_INPUT);
                      */

  ioport_set_pin_dir(_PIN_LCD_D4, IOPORT_DIR_INPUT);
  ioport_set_pin_dir(_PIN_LCD_D5, IOPORT_DIR_INPUT);
  ioport_set_pin_dir(_PIN_LCD_D6, IOPORT_DIR_INPUT);
  ioport_set_pin_dir(_PIN_LCD_D7, IOPORT_DIR_INPUT);


  ioport_set_pin_level(_PIN_LCD_RS, 0);
  ioport_set_pin_level(_PIN_LCD_RW, 1);

  do
  {
    delay_us(1);
    ioport_set_pin_level(_PIN_LCD_E, 1);
    delay_us(1);
    busy_flag = ioport_get_pin_level(_PIN_LCD_D7);
    ioport_set_pin_level(_PIN_LCD_E, 0);
    delay_us(1);
    ioport_set_pin_level(_PIN_LCD_E, 1);
    delay_us(1);
    ioport_set_pin_level(_PIN_LCD_E, 0);
  } while(busy_flag);

  /*
  ioport_set_port_dir(IOPORT_PIOC,\
                      _LCD_D4_PIO | _LCD_D5_PIO | _LCD_D6_PIO | _LCD_D7_PIO, IOPORT_DIR_OUTPUT);
                      */
  ioport_set_pin_dir(_PIN_LCD_D4, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(_PIN_LCD_D5, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(_PIN_LCD_D6, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(_PIN_LCD_D7, IOPORT_DIR_OUTPUT);

}

// LCD write high four bits
// byte ... data or command byte
void lcd_write_high(uint8_t byte){

  ioport_set_pin_level(_PIN_LCD_RW, 0);
  delay_us(1);


  ioport_set_pin_level(_PIN_LCD_D7, ( byte&(1<<7) )!=0 );
  ioport_set_pin_level(_PIN_LCD_D6, ( byte&(1<<6) )!=0 );
  ioport_set_pin_level(_PIN_LCD_D5, ( byte&(1<<5) )!=0 );
  ioport_set_pin_level(_PIN_LCD_D4, ( byte&(1<<4) )!=0 );

  ioport_set_pin_level(_PIN_LCD_E, 1);
  delay_us(1);
  ioport_set_pin_level(_PIN_LCD_E, 0);
}

// LCD write low four bits
// byte ... data or command byte
void lcd_write_low(uint8_t byte){
  delay_us(1);

  ioport_set_pin_level(_PIN_LCD_D7, ( byte&(1<<3) )!=0 );
  ioport_set_pin_level(_PIN_LCD_D6, ( byte&(1<<2) )!=0 );
  ioport_set_pin_level(_PIN_LCD_D5, ( byte&(1<<1) )!=0 );
  ioport_set_pin_level(_PIN_LCD_D4, ( byte&(1<<0) )!=0 );

  ioport_set_pin_level(_PIN_LCD_E, 1);
  delay_us(1);
  ioport_set_pin_level(_PIN_LCD_E, 0);
  busy();
}

// LCD write half command
// com ... command
void lcd_write_half_comm(uint8_t com){
  ioport_set_pin_level(_PIN_LCD_RS, 0);
  lcd_write_high(com);
}

// LCD write command
// com ... command
void lcd_write_comm(uint8_t com){
  lcd_write_half_comm(com);
  lcd_write_low(com);
}

// LCD write data
// data ... character
void lcd_write_data(uint8_t data){
  ioport_set_pin_level(_PIN_LCD_RS, 1);
  lcd_write_high(data);
  lcd_write_low(data);
}

// LCD initialisation routine
void lcd_init(void){
  ioport_init();

/*  ioport_set_port_dir(IOPORT_PIOC,\
       PIO_PC12 | PIO_PC14 | PIO_PC16 | PIO_PC13 |PIO_PC15 |PIO_PC17 |PIO_PC19,\
       IOPORT_DIR_OUTPUT);
       */

  ioport_set_pin_dir(_PIN_LCD_D4, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(_PIN_LCD_D5, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(_PIN_LCD_D6, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(_PIN_LCD_D7, IOPORT_DIR_OUTPUT);

  ioport_set_pin_dir(_PIN_LCD_E, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(_PIN_LCD_RS, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(_PIN_LCD_RW, IOPORT_DIR_OUTPUT);


  delay_us(40000);

  lcd_write_half_comm(SET | BITS_8);
  delay_us(4100);


  lcd_write_half_comm(SET | BITS_8);
  delay_us(100);
  lcd_write_half_comm(SET | BITS_8);
  busy();
  lcd_write_half_comm(SET | BITS_4);
  delay_us(100);	// LCD initialisation sometimes hangs without this delay ???
  busy();
  lcd_write_comm(SET | BITS_4 | LINES_2 | DOTS_5_8);
  lcd_write_comm(CONTROL | DISPLAY_OFF | CURSOR_OFF | BLINK_OFF);
  lcd_write_comm(CLEAR_DISPLAY);
  lcd_write_comm(MODE | CURSOR_DIRECTION_RIGHT | DISPLAY_SHIFT_OFF);
  lcd_write_comm(CONTROL | DISPLAY_ON | CURSOR_OFF | BLINK_OFF);

}


/* Sends the lcd_string to the LCD */
void lcd_driver(void){
  int i;
  lcd_write_comm(DDRAM | 0x00);
  for(i = 0; i < 16; i++){
	lcd_write_data(lcd_string[i]);
  }
  lcd_write_comm(DDRAM | 0x40);
  for(; i < 32; i++){
	lcd_write_data(lcd_string[i]);
  }
}



#ifdef __cplusplus
}
#endif
