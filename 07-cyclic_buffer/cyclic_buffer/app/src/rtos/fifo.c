#include "rtos/fifo.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t fifo_read(fifo_t *fifo, uint8_t *data, uint32_t n) {
    uint32_t bytes_read = 0;

    while (bytes_read < n && fifo->read_idx != fifo->write_idx) {
        if (fifo->read_idx >= fifo->size) {
            fifo->read_idx = 0;
        }

        *(data + bytes_read) = *(fifo->buffer + fifo->read_idx);
        fifo->read_idx++;

        bytes_read++;
    }

    return bytes_read;
}

uint32_t fifo_write(fifo_t *fifo, uint8_t *data, uint32_t n) {
    uint32_t bytes_written = 0;

    while (bytes_written < n && (fifo->write_idx != (fifo->read_idx - 1 + fifo->size) % fifo->size)) {
        if (fifo->write_idx >= fifo->size) {
            fifo->write_idx = 0;
        }

        *(fifo->buffer + fifo->write_idx) = *(data + bytes_written);
        fifo->write_idx++;

        bytes_written++;
    }

    return bytes_written;
}

#ifdef __cplusplus
}
#endif
