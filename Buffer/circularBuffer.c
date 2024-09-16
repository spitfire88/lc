// Circular Buffer that overwrites the buffer when it is full

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_BUF_LEN 10
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct circular_buffer {
    size_t head; // read from
    size_t tail; // write to
    size_t size; // current size
    char *buffer; // contains a string
} cb;

cb *initialize(void) {
    cb *c = malloc(sizeof(struct circular_buffer)); // skipping error
    c->head = 0;
    c->tail = 0;
    c->buffer = malloc(MAX_BUF_LEN * sizeof(char)); // skipping error
    return c;
}

size_t add_str(cb *c, char *s) {
    // Assumption: overwrites the buffer
    int minlen = 0; // tail -> end of buffer before circling back
    size_t copied = 0;

    minlen = MIN((MAX_BUF_LEN - c->tail), strlen(s));
    while (minlen != 0) {
        memcpy(c->buffer + c->tail, (s + copied), minlen); // skipping error
        copied += minlen;
        c->size = MIN((c->size + minlen), MAX_BUF_LEN);
        c->tail = (c->tail + minlen) % MAX_BUF_LEN;
        minlen = MIN((MAX_BUF_LEN - c->tail), strlen(s) - copied);
    }
    printf("%s\n", c->buffer);
    return copied;
}

char *del_str(cb *c, size_t len) {
    int minlen = 0;
    size_t read = 0;
    char *ret_str = malloc(len * sizeof(char));

    if (c->size == 0) {
        printf("circular buffer is empty\n");
        return NULL;
    }
    minlen = MIN((MAX_BUF_LEN - c->head), len);
    while (minlen != 0) {
        memcpy((ret_str + read), c->buffer + c->head, minlen);
        read += minlen;
        c->size = c->size - minlen;
        c->head = (c->head + minlen) % MAX_BUF_LEN;
        minlen = MIN((MAX_BUF_LEN - c->head), (len - minlen));
    }

    printf("Read %d\n",(unsigned int) read);
    return ret_str;
}

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++)
        printf("Arg %d: %s\n", argv[i]);
    
    cb *c = initialize();
    assert(c != NULL);
    assert(add_str(c, "HELLOWORLD") == strlen("HELLOWORLD"));
    assert(add_str(c, "NOMORE") == strlen("NOMORE"));
    assert(add_str(c, "HHHHHHHHHHIIIIIIIIIIJ") == strlen("HHHHHHHHHHIIIIIIIIIIJ"));
    assert(add_str(c, "K") == strlen("K"));

    assert(strcmp(del_str(c, 10), "IIIIIIJKII") == 0);
    assert(del_str(c, 1) == NULL);

    assert(add_str(c, "L") == strlen("L"));
    assert(strcmp(del_str(c, 2), "II") == 0);
    return 0;
}

