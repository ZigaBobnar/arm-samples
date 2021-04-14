#include <asf.h>
#include "lcd.h"
#include "rtos.h"
#include "clock_program.h"

#ifdef __cplusplus
extern "C" {
#endif

rtos_task_t** get_main_tasklist(void);


int main (void)
{

    /* sets the processor clock according to conf_clock.h definitions */
    sysclk_init();

    /* disable wathcdog */
    WDT->WDT_MR = WDT_MR_WDDIS;

    /********************* HW init     ***************************/
    clock_program_init();



    rtos_init(/* slice_us */ 500);
    rtos_set_tasklist_ptr(get_main_tasklist());
    rtos_enable();



    /********************* Main loop     ***************************/
    while(1)
    {
        /*clock_task.function();
        prepare_display_text_task.function();
        display_task.function();
        buttons_task.function();*/
    }



    while(1)
    {
    }
}

rtos_task_t** get_main_tasklist()
{
    return rtos_clock_program_tasks;
}

#ifdef __cplusplus
}
#endif
