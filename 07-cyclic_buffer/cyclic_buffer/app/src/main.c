#include <asf.h>

#include "rtos/fifo.h"
#include "rtos_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif



int main (void)
{
    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    rtos_tasks_init();

    rtos_init(25500);
    rtos_set_tasklist_ptr(rtos_tasklist);
    rtos_enable();


    /********************* Main loop     ***************************/
    while(1)
    {
    }


    while(1) {}
}


#ifdef __cplusplus
}
#endif
