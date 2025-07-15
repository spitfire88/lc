#include "../include/myMalloc.hpp"
#include <algorithm>
#include <cstring>

MyMalloc::MyMalloc() : current_break(memory_pool), head(nullptr) {
    // Initialize memory pool
    std::memset(memory_pool, 0, MEMORY_POOL_SIZE);
}

MyMalloc::~MyMalloc() {
    // Destructor - memory pool is automatically cleaned up
}

void* MyMalloc::alloc(size_t size) {
    if (size == 0) {
        return nullptr;
    }

    // Align size to word boundary
    size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
    size = std::max(size, MIN_BLOCK_SIZE);

    std::lock_guard<std::mutex> lock(memory_mutex);

    // Try to find a suitable free block
    Block* block = findFreeBlock(size);
    if (block != nullptr) {
        block->is_free = false;
        splitBlock(block, size);
        return reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(block) + sizeof(Block));
    }

    // Allocate a new block
    size_t total_size = size + sizeof(Block);
    if (current_break + total_size > memory_pool + MEMORY_POOL_SIZE) {
        return nullptr; // Out of memory
    }

    Block* new_block = reinterpret_cast<Block*>(current_break);
    new (new_block) Block(size);
    current_break += total_size;

    // Add to linked list
    new_block->next = head;
    head = new_block;

    return reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(new_block) + sizeof(Block));
}

void MyMalloc::free(void* ptr) {
    if (ptr == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(memory_mutex);

    // Get block metadata
    Block* block = reinterpret_cast<Block*>(reinterpret_cast<uint8_t*>(ptr) - sizeof(Block));
    
    if (!validateBlock(block)) {
        return; // Invalid block
    }

    block->is_free = true;
    coalesceBlocks();
}

void* MyMalloc::realloc(void* ptr, size_t size) {
    if (ptr == nullptr) {
        return alloc(size);
    }
    
    if (size == 0) {
        free(ptr);
        return nullptr;
    }

    // Get current block
    Block* block = reinterpret_cast<Block*>(reinterpret_cast<uint8_t*>(ptr) - sizeof(Block));
    if (!validateBlock(block)) {
        return nullptr;
    }

    size_t old_size = block->size;
    if (old_size >= size) {
        // Current block is large enough
        return ptr;
    }

    // Allocate new block and copy data
    void* new_ptr = alloc(size);
    if (new_ptr != nullptr) {
        std::memcpy(new_ptr, ptr, std::min(old_size, size));
        free(ptr);
    }

    return new_ptr;
}

Block* MyMalloc::findFreeBlock(size_t size) {
    Block* current = head;
    Block* best_fit = nullptr;
    size_t best_size = SIZE_MAX;

    while (current != nullptr) {
        if (current->is_free && current->size >= size) {
            if (current->size < best_size) {
                best_fit = current;
                best_size = current->size;
                
                // Perfect fit
                if (current->size == size) {
                    break;
                }
            }
        }
        current = current->next;
    }

    return best_fit;
}

void MyMalloc::splitBlock(Block* block, size_t size) {
    if (block->size <= size + sizeof(Block) + MIN_BLOCK_SIZE) {
        return; // Not worth splitting
    }

    // Create new block for the remaining space
    uint8_t* new_block_addr = reinterpret_cast<uint8_t*>(block) + sizeof(Block) + size;
    Block* new_block = reinterpret_cast<Block*>(new_block_addr);
    
    size_t remaining_size = block->size - size - sizeof(Block);
    new (new_block) Block(remaining_size);
    new_block->is_free = true;
    new_block->next = block->next;
    
    block->size = size;
    block->next = new_block;
}

void MyMalloc::coalesceBlocks() {
    Block* current = head;
    
    while (current != nullptr && current->next != nullptr) {
        if (current->is_free && current->next->is_free) {
            // Check if blocks are adjacent
            uint8_t* current_end = reinterpret_cast<uint8_t*>(current) + sizeof(Block) + current->size;
            uint8_t* next_start = reinterpret_cast<uint8_t*>(current->next);
            
            if (current_end == next_start) {
                // Merge blocks
                current->size += sizeof(Block) + current->next->size;
                Block* next_next = current->next->next;
                current->next = next_next;
                continue; // Don't advance current, check for more coalescence
            }
        }
        current = current->next;
    }
}

bool MyMalloc::validateBlock(Block* block) const {
    if (block == nullptr) {
        return false;
    }
    
    // Check if block is within memory pool
    uint8_t* block_addr = reinterpret_cast<uint8_t*>(block);
    if (block_addr < memory_pool || block_addr >= memory_pool + MEMORY_POOL_SIZE) {
        return false;
    }
    
    // Check magic number
    return block->magic == MAGIC_NUMBER;
}

void MyMalloc::getStats(size_t& total_allocated, size_t& total_free, float& fragmentation_ratio) const {
    std::lock_guard<std::mutex> lock(memory_mutex);
    
    total_allocated = 0;
    total_free = 0;
    size_t free_blocks = 0;
    
    Block* current = head;
    while (current != nullptr) {
        if (current->is_free) {
            total_free += current->size;
            free_blocks++;
        } else {
            total_allocated += current->size;
        }
        current = current->next;
    }
    
    // Calculate fragmentation ratio
    if (total_free > 0) {
        fragmentation_ratio = static_cast<float>(free_blocks) / total_free * 100.0f;
    } else {
        fragmentation_ratio = 0.0f;
    }
}

void MyMalloc::garbageCollect() {
    std::lock_guard<std::mutex> lock(memory_mutex);
    coalesceBlocks();
}

void MyMalloc::reset() {
    std::lock_guard<std::mutex> lock(memory_mutex);
    head = nullptr;
    current_break = memory_pool;
    std::memset(memory_pool, 0, MEMORY_POOL_SIZE);
}
