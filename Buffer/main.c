#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

typedef struct {
    __uint32_t head;
    __uint32_t tail;
    __uint32_t size;
    char *buffer;
} ring_buffer_t;

ring_buffer_t *ring_buffer_init(int size) {
    ring_buffer_t *rb = (ring_buffer_t *)malloc(sizeof(ring_buffer_t));
    if (rb == NULL) {
        return NULL;
    }
    rb->head = 0;
    rb->tail = 0;
    rb->size = size;
    rb->buffer = (char *)aligned_alloc(getpagesize(), size);
    if (rb->buffer == NULL) {
        free(rb);
        return NULL;
    }
    return rb;
}

void ring_buffer_free(ring_buffer_t *rb) {
    free(rb->buffer);
    free(rb);
}

int ring_buffer_memcpy_write(ring_buffer_t *rb, char *data, int len) {
    int copy_len = (rb->size - rb->head - 1) > len ? len : (rb->size - rb->head - 1) ;

    memcpy(rb->buffer + rb->head, data, copy_len);
    if (copy_len < len) {
        memcpy(rb->buffer, data + copy_len, len - copy_len);
        rb->head = len - copy_len;
    } else {
        rb->head += len;
    }

    return len;
}

int ring_buffer_read(ring_buffer_t *rb, char *data, int len) {
    // empty buffer case
    if (rb->tail == rb->head) {
        memset(data, 0, len);
        return 0;
    }

    // len is greater than buffer size case
    if (len > rb->size) {
        len = rb->size;
    } 
    
    // len is greater than available data case
    if (len > (rb->head - rb->tail + 1)) {
        len = rb->head - rb->tail + 1;
    }

    int to_read = (rb->size - rb->tail) > len ? len : (rb->size - rb->tail);

    strncpy(data, rb->buffer + rb->tail, to_read);
    if (to_read < len) {
        strncpy(data + to_read, rb->buffer, len - to_read);
        rb->tail = len - to_read;
    } else {
        rb->tail += len;
    }

    return len;
}

int main(
        int argc,
        char **argv
    ) 
{
    ring_buffer_t *rb = ring_buffer_init(4096);
    assert(rb != NULL);

    char data[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ12";
    char *read_data = malloc(rb->size * sizeof(char));

    int len = ring_buffer_memcpy_write(rb, data, strlen(data));
    assert(len == strlen(data));
    printf("wrote %d bytes, rb is %s\n", len, rb->buffer);

    memset(read_data, 0, rb->size);
    len = ring_buffer_read(rb, read_data, 10);
    assert(len <= 10);
    printf("read %d bytes(%s), rb is %s\n", len, read_data, rb->buffer);

    memset(read_data, 0, rb->size);
    len = ring_buffer_read(rb, read_data, 100);
    assert(len <= 100);
    printf("read %d bytes(%s), rb is %s\n", len, read_data, rb->buffer);

    len = ring_buffer_memcpy_write(rb, data, strlen(data));
    assert(len == strlen(data));
    printf("wrote %d bytes, rb is %s\n", len, rb->buffer);

    memset(read_data, 0, rb->size);
    len = ring_buffer_read(rb, read_data, 4097);
    assert(len <= 4096);
    printf("read %d bytes(%s), rb is %s\n", len, read_data, rb->buffer);

    memset(read_data, 0, rb->size);
    len = ring_buffer_read(rb, read_data, 4097);
    assert(len <= 4096);
    printf("read %d bytes(%s), rb is %s\n", len, read_data, rb->buffer);

    ring_buffer_free(rb);

    return 0;
}