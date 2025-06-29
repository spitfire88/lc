#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define MEMORY_POOL_SIZE 1024 * 1024 // 1 MB

typedef struct Block {
    size_t size;         // Size of the block
    bool is_free;        // Whether the block is free
    struct Block *next;  // Pointer to the next block
} Block;

static uint8_t memory_pool[MEMORY_POOL_SIZE];
static Block *head = NULL; // Head of the linked list of blocks

// Mutex for thread safety
static pthread_mutex_t memory_mutex = PTHREAD_MUTEX_INITIALIZER;

void *my_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    pthread_mutex_lock(&memory_mutex); // Lock the mutex

    Block *current = head;
    Block *prev = NULL;

    // Search for a free block that is large enough
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            current->is_free = false;
            pthread_mutex_unlock(&memory_mutex); // Unlock the mutex
            return (void *)(current + 1); // Return memory after the metadata
        }
        prev = current;
        current = current->next;
    }

    // Allocate a new block if no suitable free block is found
    size_t total_size = size + sizeof(Block);
    if ((uint8_t *)memory_pool + MEMORY_POOL_SIZE - (uint8_t *)memory_pool < total_size) {
        pthread_mutex_unlock(&memory_mutex); // Unlock the mutex
        return NULL; // Out of memory
    }

    Block *new_block = (Block *)((prev == NULL) ? memory_pool : (uint8_t *)(prev + 1) + prev->size);
    new_block->size = size;
    new_block->is_free = false;
    new_block->next = NULL;

    if (prev != NULL) {
        prev->next = new_block;
    } else {
        head = new_block;
    }

    pthread_mutex_unlock(&memory_mutex); // Unlock the mutex
    return (void *)(new_block + 1); // Return memory after the metadata
}

void my_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    pthread_mutex_lock(&memory_mutex); // Lock the mutex

    Block *block = (Block *)ptr - 1; // Get the metadata for the block
    block->is_free = true;

    pthread_mutex_unlock(&memory_mutex); // Unlock the mutex
}

int main() {
    // Example usage of my_malloc and my_free
    int *arr = (int *)my_malloc(10 * sizeof(int));
    if (arr == NULL) {
        printf("Allocation failed\n");
        return 1;
    }

    // Use the allocated memory
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Free the memory
    my_free(arr);

    // Allocate again to test reuse
    int *arr2 = (int *)my_malloc(5 * sizeof(int));
    if (arr2 == NULL) {
        printf("Allocation failed\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        arr2[i] = i * 2;
    }

    for (int i = 0; i < 5; i++) {
        printf("%d ", arr2[i]);
    }
    printf("\n");

    my_free(arr2);

    return 0;
}