#include <asf.h>

#ifndef RTOS_H_
#define RTOS_H_

typedef void (*ptr_function)(void);

struct _task {
    ptr_function function; // Kazalec na funkcijo
    uint32_t last_tick; // Kdaj je bila funkcija nazadnje zagnana
};

typedef struct _task rtos_task_t;

uint32_t rtos_init(uint32_t slice_us);
void rtos_disable(void);
void rtos_enable(void);

void rtos_set_tasklist_ptr(rtos_task_t* (*tasklist_ptr), uint32_t tasks_count);

#endif  // RTOS_H_
