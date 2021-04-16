#ifndef LIB_DRIVERS_LCD_H_
#define LIB_DRIVERS_LCD_H_

/**
 * LCD driver for 16x2 character display
 * It can display titak of 32 ASCII characters, 16 per row.
 * Those displays are based on Hitachi HD44780 chipset.
 * 
 * Physical connections:
 * PIN1 -> GND
 * PIN2 -> VCC
 * PIN3 -> Vo - contrast of the lcd
 * PIN4 -> RS - Register select (0 - commands, 1 - data)
 * PIN5 -> RW - Read or write to lcd
 * PIN6 -> E - Enable the lcd chip
 * PIN7-PIN14 -> D0-D7 - Data/command bus. Data can also be split in two parts and sent only through D4-D7, this reduces the pin usage but can be slower
 * PIN15 -> (Optional) A - Anode for LED backlight - Positive terminal
 * PIN16 -> (Optional) K - Cathode for LED backlight - Negative terminal
 */

#include "../common.h"

__EXTERN_C_OPEN

#include <ioport.h>

/**
 * Pin definitions
 */
#define _PIN_LCD_D4	PIO_PC13_IDX    // Due pin 50,	SAM3X8E pin PC13
#define _PIN_LCD_D5	PIO_PC15_IDX    // Due pin 48,	SAM3X8E pin PC15
#define _PIN_LCD_D6	PIO_PC17_IDX    // Due pin 46,	SAM3X8E pin PC17
#define _PIN_LCD_D7	PIO_PC19_IDX    // Due pin 44,	SAM3X8E pin PC19
#define _PIN_LCD_RS	PIO_PC12_IDX    // Due pin 51, 	SAM3X8E pin PC12
#define _PIN_LCD_RW	PIO_PC14_IDX    // Due pin 49,	SAM3X8E pin PC14
#define _PIN_LCD_E	PIO_PC16_IDX    // Due pin 47	SAM3X8E pin PC16

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

struct _lcd {
    /** R/S pin (0: command, 1: character) */
    uint32_t rs;
    /** R/W pin (0: write to LCD, 1: read from LCD) */
    uint32_t rw;
    /** Enable pin (1: enabled) */
    uint32_t enable;

    uint32_t d4;
    uint32_t d5;
    uint32_t d6;
    uint32_t d7;

    //char lcd_buffer[33];
    //char* lcd_string;
};
typedef struct _lcd lcd_t;

/**
 * Initialize LCD driver
 */
void lcd_init(lcd_t* lcd);

/**
 * Write contents of lcd_string to the display
 */
void lcd_write_string(lcd_t* lcd);

/**
 * Clear the LCD screen
 */
void lcd_clear(lcd_t* lcd);

/**
 * Write command to the LCD
 */
void lcd_write_command(lcd_t* lcd, uint8_t command);

/**
 * Send value to the LCD
 * @param is_command Determines whether the value should be sent as command or as characters
 */
void lcd_send(lcd_t* lcd, uint8_t value, bool is_command);

/**
 * Sends a pulse (0, 1, 0) to the enable pin of LCD
 */
void lcd_pulse_enable(lcd_t* lcd);

extern char *lcd_string;

__EXTERN_C_CLOSE

#endif  // LIB_DRIVERS_LCD_H_
