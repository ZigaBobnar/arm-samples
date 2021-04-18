#ifndef LIB_RTOS_FIFO_H_
#define LIB_RTOS_FIFO_H_

#include <asf.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _fifo {
    uint32_t read_idx;
    uint32_t write_idx;
    uint32_t size;
    uint8_t *buffer;
};

typedef struct _fifo fifo_t;

/**
 * Read from fifo buffer
 * @param fifo Fifo buffer to read data from
 * @param data_buffer Pointer to where fifo data should be read
 * @param n Number of bytes to read from buffer
 * @returns Number of bytes that were read
 */
uint32_t fifo_read(fifo_t* fifo, uint8_t* data_buffer, uint32_t n);

/**
 * Peeks (reads without advancing read pointer) the fifo buffer
 * @param fifo Fifo buffer to read data from
 * @param data_buffer Pointer to where fifo data should be read
 * @param n Number of bytes to read from buffer
 * @returns Number of bytes that were read
 */
uint32_t fifo_peek(fifo_t* fifo, uint8_t* data_buffer, uint32_t n);

/**
 * Write data into fifo buffer
 * @param fifo Fifo buffer to write data to
 * @param data_ptr Pointer to data that needs to be written into fifo buffer
 * @param n Number of bytes to write to buffer
 * @returns Number of bytes that were written
 */
uint32_t fifo_write(fifo_t *fifo, uint8_t *data_ptr, uint32_t n);

/**
 * Write single byte into buffer
 * @param fifo Fifo buffer to write data to
 * @param value Value that needs to be written into fifo buffer
 * @returns Number of bytes that were written
 */
uint32_t fifo_write_single(fifo_t *fifo, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif  // LIB_RTOS_FIFO_H_
