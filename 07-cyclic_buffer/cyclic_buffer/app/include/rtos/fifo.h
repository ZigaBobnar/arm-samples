#ifndef FIFO_H_
#define FIFO_H_

#include <asf.h>

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
 * @param data Pointer to where fifo data should be read
 * @param n Number of bytes to read from buffer
 * @return Number of bytes that were read
 */
uint32_t fifo_read(fifo_t *fifo, uint8_t *data, uint32_t n);

/**
 * Write data into fifo buffer
 * @param fifo Fifo buffer to write data to
 * @param data Pointer to data that needs to be written into fifo buffer
 * @param n Number of bytes to write to buffer
 * @return Number of bytes that were written
 */
uint32_t fifo_write(fifo_t *fifo, uint8_t *data, uint32_t n);

#endif  // FIFO_H_
