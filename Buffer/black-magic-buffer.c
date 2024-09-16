#include "blackMagicBuffer.h"

bool putmcpy(queue_t *q, uint8_t *data, size_t size) {
    if(q->buffer_size - q->bytes_avail < size) {
        return false;
    }

    const size_t part1 = min(size, q->buffer_size - q->tail);
    const size_t part2 = size - part1;

    memcpy(q->buffer + q->tail, data, part1);
    memcpy(q->buffer, data + part1, part2);

    q->tail = (q->tail + size) % q->buffer_size;
    q->bytes_avail += size;
    return true;
}

bool getmcpy(queue_t *q, uint8_t *data, size_t size) {
    if(q->bytes_avail < size) {
        return false;
    }

    const size_t part1 = min(size, q->buffer_size - q->head);
    const size_t part2 = size - part1;

    memcpy(data, q->buffer + q->head, part1);
    memcpy(data + part1, q->buffer, part2);

    q->head = (q->head + size) % q->buffer_size;
    q->bytes_avail -= size;
    return true;
}

int main() {
    uint8_t data[10] = { 1, 1, 1, 1 };
    size_t size = 0;
    queue_t *t = initq(10);
    
    
    return 0;
}