#include <rtos/rtos.h>

#ifdef __cplusplus
extern "C" {
#endif


rtos_task_t* (*_tasklist_ptr) = NULL;
rtos_task_t** _current_task_ptr = NULL;
uint32_t total_ticks_run = 0;


uint32_t rtos_init(uint32_t slice_us)
{
    // Get correct number of "ticks" per slice:
    uint32_t n_of_cycles_per_slice = sysclk_get_main_hz() / 1000 / 1000 * slice_us;

    SysTick->LOAD = n_of_cycles_per_slice;


    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk) &
                   ~SysTick_CTRL_ENABLE_Msk;

    NVIC_EnableIRQ(SysTick_IRQn);

    NVIC_SetPriority(SysTick_IRQn, 0);

    return 0;
}

void rtos_disable(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void rtos_enable(void)
{
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void rtos_set_tasklist_ptr(rtos_task_t* (*tasklist_ptr))
{
    _tasklist_ptr = tasklist_ptr;
    _current_task_ptr = _tasklist_ptr;
}

void SysTick_Handler(void)
{
    // TODO
    // System control block -> Interrupt Control and State Register -> Check if SysTick interrupt is pending
    /*if (SCB->ICSR & SCB_ICSR_PENDSTSET_Msk) {
    }*/

    // The tasklist must be terminated with "null function" task.
    if ((*_current_task_ptr)->function == NULL) {
        _current_task_ptr = _tasklist_ptr;
    }

    // Increases for next task, ccurrent task will be actually run.
    // This will prevent problems if increase happened after the task was run as it might actually not happen.
    rtos_task_t* running_task = *_current_task_ptr;
    ++_current_task_ptr;

    running_task->function();

    running_task->last_tick = total_ticks_run;
    total_ticks_run++;
}

#ifdef __cplusplus
}
#endif
