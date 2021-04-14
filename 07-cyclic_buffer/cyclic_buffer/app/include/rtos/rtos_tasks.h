#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include <asf.h>
#include "fifo.h"

#define KEYS_FIFO_SIZE 10 // Size for fifo keys (9 in this case)
extern fifo_t keys_fifo;

#endif  // RTOS_TASKS_H_
