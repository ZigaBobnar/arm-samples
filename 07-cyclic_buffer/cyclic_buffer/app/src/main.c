#include <asf.h>

#include "rtos/fifo.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t char_buffer[16] = "$NTJRGFKALN$L#T";
fifo_t char_fifo = { 0, 0, 16, char_buffer };

int main (void)
{
    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/

    uint32_t n;
    uint8_t char_write = 'r', char_read;
    n = fifo_write(&char_fifo, &char_write, 1);
    n = fifo_read(&char_fifo, &char_read, 1);
    n = fifo_write(&char_fifo, "test", 5);

    uint8_t test_buffer[8];
    n = fifo_read(&char_fifo, test_buffer, 8);




    /********************* Main loop     ***************************/
    while(1)
    {
    }


    while(1) {}
}


#ifdef __cplusplus
}
#endif
