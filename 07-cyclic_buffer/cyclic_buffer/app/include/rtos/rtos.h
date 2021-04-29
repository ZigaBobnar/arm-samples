#ifndef RTOS_H_
#define RTOS_H_

#include <asf.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Pointer to void argument, void return function
 */
typedef void (*ptr_function)(void);

/**
 * RTOS task instance
 */
struct _task {
    /** Function pointer to execute in this task */
    ptr_function function;

    /** Last function execution tick */
    uint32_t last_tick;

    const char* task_name;
};
/** RTOS task instance */
typedef struct _task rtos_task_t;

/**
 * Initialize RTOS system. Note: this will not start the system yet, you need to call rtos_enable()
 * @param slice_us Time in microseconds before next slice is run
 */
uint32_t rtos_init(uint32_t slice_us);

/**
 * Stops the RTOS system
 */
void rtos_disable(void);

/**
 * Starts the RTOS system
 */
void rtos_enable(void);

/**
 * Sets the pointer to tasks that RTOS will be running.
 * Tasklist MUST be terminated with an element that has NULL function pointer to mark an end of the list.
 */
void rtos_set_tasklist_ptr(rtos_task_t* (*tasklist_ptr));

#ifdef __cplusplus
}
#endif

#endif  // RTOS_H_
