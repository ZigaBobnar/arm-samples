#include "rtos/rtos_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t keys_buff[KEYS_FIFO_SIZE];
fifo_t keys_fifo = { 0, 0, KEYS_FIFO_SIZE, keys_buff };

#ifdef __cplusplus
}
#endif
