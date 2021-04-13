/*
 * test_board.h
 *
 *  Created on: Jan 11, 2017
 *      Author: jernej
 */

#ifndef TEST_BOARD_H_
#define TEST_BOARD_H_


/*
 * Everything needed for custom made HW board (LCD, 4xLED, 4xbuttons, pot)
 * LCD is a separate module (lcd.h, lcd.c)
 *
 * e.g. ioport_set_pin_level(L0, !ioport_get_pin_level(BTN0));
 */


#include <asf.h>

#define F_CPU SystemCoreClock	// needed by asf/common/services/delay
#include <delay.h>

#include <adc.h>

/* LCD stuff is included from lcd.h */
#include <lcd.h>


/* These pins can be reconfigured */
#define D1 PIO_PC22_IDX
#define D2 PIO_PC21_IDX
#define D3 PIO_PC29_IDX
#define D4 PIO_PD7_IDX

#define T1 PIO_PC26_IDX	// pio pin id
#define T2 PIO_PC25_IDX
#define T3 PIO_PC24_IDX
#define T4 PIO_PC23_IDX

#define T1_MSK 1<<0	// bit mask
#define T2_MSK 1<<1
#define T3_MSK 1<<2
#define T4_MSK 1<<3

void init_buttons_and_leds(void);
uint32_t get_buttons_state(void);
uint32_t get_buttons_press(void);


#define ADC_CHANNEL ADC_CHANNEL_7	/* ADC_CHANNEL_7 (PA16): Arduino Pin A0 */
void adc_setup(void);
uint32_t adc_read(void);

#endif /* TEST_BOARD_H_ */
