#include <rtos.h>

#ifdef __cplusplus
extern "C" {
#endif


rtos_task_t* (*_tasklist_ptr) = NULL;
uint32_t _tasks_count = 0;
char/*uint32_t*/ _current_task_idx = 0;


uint32_t rtos_init(uint32_t slice_us)
{
    sysclk_set_source(SYSCLK_SRC_MAINCK_8M_RC);
    SysTick->LOAD = sysclk_get_main_hz() / slice_us;

    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;

    NVIC_EnableIRQ(SysTick_IRQn);

    NVIC_SetPriority(SysTick_IRQn, 1);

    return 0;
}

void rtos_disable(void)
{
    // TODO
}

void rtos_enable(void)
{
    // TODO
}

void rtos_set_tasklist_ptr(rtos_task_t* (*tasklist_ptr), uint32_t tasks_count)
{
    _tasklist_ptr = tasklist_ptr;
    _tasks_count = tasks_count;
}


void SysTick_Handler(void)
{
    if (++_current_task_idx > _tasks_count) {
        _current_task_idx = 0;
    }

    (*(_tasklist_ptr + _current_task_idx))->function();
}

#ifdef __cplusplus
}
#endif
