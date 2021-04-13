/*
 * test_board.c
 *
 *  Created on: Jan 11, 2017
 *      Author: jernej
 */

#include <test_board.h>

/* Initialize buttons and LEDs */
void init_buttons_and_leds(void){
  ioport_init();	/* enables all PIO ports */

  ioport_enable_pin(D1);
  ioport_enable_pin(D2);
  ioport_enable_pin(D3);
  ioport_enable_pin(D4);
  ioport_enable_pin(T1);
  ioport_enable_pin(T2);
  ioport_enable_pin(T3);
  ioport_enable_pin(T4);
  
  ioport_set_pin_dir(D1, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(D2, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(D3, IOPORT_DIR_OUTPUT);
  ioport_set_pin_dir(D4, IOPORT_DIR_OUTPUT);

  ioport_set_pin_level(D1, 0);
  ioport_set_pin_level(D2, 0);
  ioport_set_pin_level(D3, 0);
  ioport_set_pin_level(D4, 0);

  /* pullups are enabled by default => reconfig not really needed */
  ioport_set_pin_dir(T1, IOPORT_DIR_INPUT);
  ioport_set_pin_dir(T2, IOPORT_DIR_INPUT);
  ioport_set_pin_dir(T3, IOPORT_DIR_INPUT);
  ioport_set_pin_dir(T4, IOPORT_DIR_INPUT);

  ioport_set_pin_mode(T1, IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE);
  ioport_set_pin_mode(T2, IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE);
  ioport_set_pin_mode(T3, IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE);
  ioport_set_pin_mode(T4, IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE);
}

uint32_t get_buttons_state(void){
  /*
   * returns the logic state of buttons (buttons are active 0)
   * 	bit0 => BTN0, ... bit3 => BTN3
   */
  return (0xFFFFFFF0 | ioport_get_pin_level(T1) << 0 | ioport_get_pin_level(T2)<<1
      | ioport_get_pin_level(T3)<<2  | ioport_get_pin_level(T4)<<3);
}

uint32_t get_buttons_press(void){
/*
 * returns bitmask of pressed (failling edge) keys (bit0 => btn0,...)
 * 	bit0 = 1 : button 0 was pressed (edge detected between two calls)
 * 	bit0 = 0 : button 0 not changed
 */
  static uint32_t btn_old = 0;

  uint32_t btn = get_buttons_state();
  uint32_t out = (btn ^ btn_old) & (btn_old);
  btn_old = btn;
  return out;
}

void adc_setup(void){
    /* Initializes ADC channel as define by ADC_CHANNEL macro
     * ADC pio assignment is automatic when channel is enabled => no need for PIO reconfig
     */
    sysclk_enable_peripheral_clock(ID_ADC);
    adc_init(ADC, SystemCoreClock, 6400000, ADC_STARTUP_TIME_4);
    adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
    adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_12);
    adc_enable_channel(ADC, ADC_CHANNEL);
    adc_configure_trigger(ADC, ADC_TRIG_SW, 0);
}

uint32_t adc_read(void){
  adc_start(ADC);	/* start the conversion */
  uint32_t status, ad_val;
  do{
	  status = adc_get_channel_status(ADC, ADC_CHANNEL);
  }while( status == 0);
  ad_val = adc_get_channel_value(ADC, ADC_CHANNEL);
  return ad_val;
}

