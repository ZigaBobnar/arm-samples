#ifndef CLOCK_PROGRAM_H_
#define CLOCK_PROGRAM_H_

#include "rtos.h"

void clock_program_init(void);

extern rtos_task_t clock_task;
extern rtos_task_t prepare_display_text_task;
extern rtos_task_t display_task;
extern rtos_task_t buttons_task;

extern rtos_task_t *rtos_clock_program_tasks[];

#endif  // CLOCK_PROGRAM_H_
