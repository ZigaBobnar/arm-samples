#include <asf.h>

#ifdef __cplusplus
extern "C" {
#endif

bool output_to_serial = false;

int main (void)
{
    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    sysclk_enable_peripheral_clock(ID_PIOA);
    sysclk_enable_peripheral_clock(ID_UART);

    pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA8A_URXD | PIO_PA9A_UTXD);
    pio_pull_up(PIOA, PIO_PA8A_URXD | PIO_PA9A_UTXD, PIO_PULLUP);

    sam_uart_opt_t uart_opts = {
        .ul_mck = SystemCoreClock,
        .ul_baudrate = 115200,
        .ul_mode = UART_MR_CHMODE_NORMAL | UART_MR_PAR_NO,
    };
    uart_init(UART, &uart_opts);

    // uart_write(UART, 'A');

    UART->UART_CR = UART_CR_RSTSTA | UART_CR_RSTTX | UART_CR_RSTRX;
    uart_enable_interrupt(UART, UART_IER_RXRDY); // UART->UART_IER = UART_IER_RXRDY
    NVIC_EnableIRQ(UART_IRQn);

    uart_enable(UART); // UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN

    /********************* Main loop     ***************************/
    while(1)
    {
        // Blocking version:
        /*
        uint8_t value;
        if (uart_read(UART, &value) == 0) {
            if (value == 27) {
                output_to_serial = !output_to_serial;
            }

            if (output_to_serial) {
                uart_write(UART, value);
            } else {
                // lcd_write_char(value);
            }
        }
        */
    }



    while(1) {}
}

void UART_Handler(void) {
    if (uart_get_status(UART) != 0) {
        // Error.
    }

    if (UART->UART_IMR & UART_IMR_RXRDY/* && UART->UART_SR & UART_SR_RXRDY*/) {
        uint8_t value = UART->UART_RHR;

        if (value == 27) {
            output_to_serial = !output_to_serial;
        }

        if (output_to_serial) {
            UART->UART_THR = value;
        } else {
            // lcd_write_char(value);
        }
    }
}

#ifdef __cplusplus
}
#endif
