// There is an available memory pool that malloc will allocate memory from.
// If I need to free it and reuse, then we need to use metadata to identify each allocated block
// For thread-safe malloc, use mutexes

#include <stdio.h>
#include <pthread.h> // mutex
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define MEMORY_POOL_SIZE 1024 * 1024 // 1MB

// Metadata
typedef struct Block {
    size_t size;
    bool is_free;
    struct Block *next; // pointer to the next block
} Block;

static uint8_t memory_pool[MEMORY_POOL_SIZE]; // contiguous 
static uint8_t *current_break = memory_pool;
static Block *head = NULL;

static pthread_mutex_t memory_mutex = PTHREAD_MUTEX_INITIALIZER;

void *my_malloc(size_t size) {
    if(size == 0) {
        return NULL;
    }

    pthread_mutex_lock(&memory_mutex);

    Block *current = head;
    Block *prev = NULL;

    // Search for a free block that is large enough
    while(current != NULL) {
        if(current->is_free && current->size >= size) {
            current->is_free = false;
            pthread_mutex_unlock(&memory_mutex);

            return (void *)(current + 1); // return memory after metadata
        }
        prev = current;
        current = current->next;
    }

    // Allocate a new block if no suitable free block was found
    size_t total_size = size + sizeof(Block);
    if ((uint8_t *)memory_pool + MEMORY_POOL_SIZE < current_break + total_size) {
        pthread_mutex_unlock(&memory_mutex);
        return NULL; // Out of memory
    }

    Block *new_block = (Block *)current_break;
    new_block->size = size;
    new_block->is_free = false;
    new_block->next = NULL;
    current_break = current_break + total_size;

    if (prev != NULL) {
        prev->next = new_block;
    } else {
        head = new_block;
    }

    pthread_mutex_unlock(&memory_mutex);
    return (void *)(new_block + 1);
}

void my_free(void *ptr) {
    if(ptr == NULL) {
        return;
    }

    pthread_mutex_lock(&memory_mutex);

    Block *block = (Block *) ptr - 1; // Get the metadata for the block
    block->is_free = true;

    pthread_mutex_unlock(&memory_mutex);
}

int main() {
    int *arr = my_malloc(10 * sizeof(int));
    if(arr == NULL) {
        printf("arr was NULL\n");
    }
    //assert(arr == NULL);
    char *str = my_malloc(400 * sizeof(char));
    if(str == NULL) {
        printf("arr was NULL\n");
    }
    //assert(str == NULL);
    my_free(str);
    //assert(str != NULL);
    int *arr2 = my_malloc(100 * sizeof(int));
    //assert(arr2 == NULL);
    int *arr3 = my_malloc(MEMORY_POOL_SIZE * sizeof(int));
    //assert(arr3 != NULL);
    return 0;
}