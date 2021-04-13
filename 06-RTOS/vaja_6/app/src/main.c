#include <asf.h>
#include <rtos.h>

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
    rtos_init(500);
    rtos_set_tasklist_ptr(get_main_tasklist(), 1);


    /********************* Main loop     ***************************/
    while(1)
    {
    }
}


// RTOS:

void lcd_driver(void);

void lcd_driver() {}

rtos_task_t lcd_task = {
    .function = lcd_driver, // pika je pomembna!
    .last_tick = 0,
};

rtos_task_t *rtos_task_list[] = {
    &lcd_task,
};

rtos_task_t** get_main_tasklist()
{
    return rtos_task_list;
}


#ifdef __cplusplus
}
#endif
