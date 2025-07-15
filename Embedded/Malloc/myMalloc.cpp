#include <iostream>
#include <cstdint>
#include <cstdbool>
#include <mutex>

// class myMalloc
// pool [contiguous 1MB]
// free? yes, metadata {size, is_free, *next}
// fragmentation? yes, simple? reallocate free blocks if size <=
// memory compaction? yes, compaction(rearrange free block, coalesce)
// thread safe? yes, mutex
// performance -> finer grained locking, per block

class myMalloc {
private:
    static const size_t MEM_POOL_SIZE = 1024 * 1024; // 1MB
    uint8_t memory_pool[MEM_POOL_SIZE]; // contiguous
    uint8_t *current_break;
    struct metaData {
        size_t size;
        bool is_free;
        metaData *next;
    };
    metaData *head;

public:
    myMalloc() : current_break(memory_pool), head(nullptr) {}

    void *alloc(size_t size) {
        if(size == 0) {
            return nullptr;
        }

        metaData *current = head;
        metaData *prev = nullptr;
        while(current != nullptr) {
            if(current->is_free && current->size >= size) {
                current->is_free = false;
                return reinterpret_cast<void *>(current + 1);
            }
            current = current->next;
            prev = current;
        }
        // Allocate
        size_t total_size = size + sizeof(metaData);
        if(current_break + total_size > memory_pool + MEM_POOL_SIZE) {
            std::cerr << "Out of Memory!\n";
            return nullptr;
        }

        metaData *new_block = reinterpret_cast<metaData *>(current_break);
        new_block->is_free = false;
        new_block->size = size;
        new_block->next = nullptr;

        current_break += total_size;

        if(prev != nullptr) {
            prev->next = new_block;
        } else {
            head = new_block;
        }
        return reinterpret_cast<void *>(new_block + 1);
    }

    void free(void **ptr) {
        if(*ptr == nullptr) {
            return;
        }
        metaData *block = reinterpret_cast<metaData *>(*ptr) - 1;
        block->is_free = true;

        // coalesce consecutive free blocks
        metaData *current = head;
        while(current != nullptr) {
            if (current->is_free && current->next != nullptr && current->next->is_free) {
                current->size += sizeof(metaData) + current->next->size;
                current->next = current->next->next;
            } else {
                current = current->next;
            }
        }
        *ptr = nullptr;
    }
};