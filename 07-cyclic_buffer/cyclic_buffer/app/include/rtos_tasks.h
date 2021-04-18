#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include <asf.h>
#include "rtos/fifo.h"
#include "rtos/rtos.h"
#include "drivers/lcd.h"

#ifdef __cplusplus
extern "C" {
#endif

extern lcd_t lcd;
extern fifo_t button_presses_fifo;

void rtos_tasks_init(void);

extern rtos_task_t buttons_task;
extern rtos_task_t led_driver_task;
extern rtos_task_t lcd_driver_task;

#define KEYS_FIFO_SIZE 10 // Size for fifo keys (9 in this case)
extern fifo_t keys_fifo;

extern rtos_task_t* rtos_tasklist[];

#ifdef __cplusplus
}
#endif

#endif  // RTOS_TASKS_H_
