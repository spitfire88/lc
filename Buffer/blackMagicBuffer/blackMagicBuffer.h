#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>

static inline off_t min(off_t a, off_t b) {
    return a < b ? a : b;
}

typedef struct {
    uint8_t *buffer;
    size_t buffer_size;
    size_t head;
    size_t tail;
    size_t bytes_avail;
} queue_t;

queue_t *initq(size_t size) {
    queue_t *q = malloc(sizeof(queue_t));
    q->head = q->tail = q->bytes_avail = 0;
    q->buffer_size = size;
    q->buffer = malloc(sizeof(uint8_t) * size);
    return q;
}