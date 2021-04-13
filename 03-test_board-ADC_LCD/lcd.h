/*
 * lcd.h
 *
 *  Created on: Sep 23, 2016
 *      Author: vaje
 */

#ifndef LCD_H_
#define LCD_H_



#ifdef __cplusplus
extern "C" {
#endif

#include <delay.h>
#include <ioport.h>

// LCD pins
// pin 3 (VEE): 0-5V to change contrast

#define _PIN_LCD_D4	PIO_PC13_IDX	// Arduino pin 50,	SAM3X8E pin PC13
#define _PIN_LCD_D5	PIO_PC15_IDX	// Arduino pin 48,	SAM3X8E pin PC15
#define _PIN_LCD_D6	PIO_PC17_IDX	// Arduino pin 46,	SAM3X8E pin PC17
#define _PIN_LCD_D7	PIO_PC19_IDX	// Arduino pin 44,	SAM3X8E pin PC19
#define _PIN_LCD_RS	PIO_PC12_IDX	// Arduino pin 51, 	SAM3X8E pin PC12
#define _PIN_LCD_RW	PIO_PC14_IDX	// Arduino pin 49,	SAM3X8E pin PC14
#define _PIN_LCD_E	PIO_PC16_IDX	// Arduino pin 47	SAM3X8E pin PC16



// LCD commands
#define CLEAR_DISPLAY				0x01
#define RETURN_HOME				0x02
#define MODE					0x04
#define CURSOR_DIRECTION_RIGHT			0x02	// MODE command flag
#define CURSOR_DIRECTION_LEFT			0x00	// MODE command flag
#define DISPLAY_SHIFT_ON			0x01	// MODE command flag
#define DISPLAY_SHIFT_OFF			0x00	// MODE command flag
#define CONTROL					0x08
#define DISPLAY_ON				0x04	// CONTROL command flag
#define DISPLAY_OFF				0x00	// CONTROL command flag
#define CURSOR_ON				0x02	// CONTROL command flag
#define CURSOR_OFF				0x00	// CONTROL command flag
#define BLINK_ON				0x01	// CONTROL command flag
#define BLINK_OFF				0x00	// CONTROL command flag
#define SHIFT					0x10
#define DISPLAY					0x08	// SHIFT command flag
#define CURSOR					0x00	// SHIFT command flag
#define RIGHT					0x04	// SHIFT command flag
#define LEFT					0x00	// SHIFT command flag
#define SET					0x20
#define BITS_8					0x10	// SET command flag
#define BITS_4					0x00	// SET command flag
#define LINES_2					0x08	// SET command flag
#define LINE_1					0x00	// SET command flag
#define DOTS_5_10				0x04	// SET command flag
#define DOTS_5_8				0x00	// SET command flag
#define CGRAM					0x40
#define DDRAM					0x80

// Get LCD cursor position
uint8_t lcd_get_cursor(void);

// Wait for busy state of LCD to end
void busy(void);

// LCD write high four bits
// byte ... data or command byte
void lcd_write_high(uint8_t byte);

// LCD write low four bits
// byte ... data or command byte
void lcd_write_low(uint8_t byte);

// LCD write half command
// com ... command
void lcd_write_half_comm(uint8_t com);

// LCD write command
// com ... command
void lcd_write_comm(uint8_t com);

// LCD write data
// data ... character
void lcd_write_data(uint8_t data);

// LCD initialisation routine
void lcd_init(void);


extern char *lcd_string;
void lcd_driver(void);

#endif /* LCD_H_ */

#ifdef __cplusplus
}
#endif
