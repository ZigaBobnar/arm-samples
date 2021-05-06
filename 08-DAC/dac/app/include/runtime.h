#ifndef RUNTIME_H_
#define RUNTIME_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void buttons_handler_init(void);
uint8_t buttons_get_state(void);
void buttons_handle(void);

void adc_handler_init(void);
uint32_t adc_read_value(void);
void adc_handle(void);

void dac_handler_init(void);
void dac_prepare_wave_tables(void);
void dac_handle(void);
void dac_update_frequency(void);

void lcd_handler_init(void);
void lcd_handle(void);

typedef enum {
    SIGNAL_SINE, SIGNAL_TRIANGULAR
} signal_type;

extern signal_type g_current_signal_type;
extern float g_frequency;
extern int g_current_signal_value_index;
extern int g_signal_direction;

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // RUNTIME_H_
