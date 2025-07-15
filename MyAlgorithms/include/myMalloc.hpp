#ifndef MY_MALLOC_HPP
#define MY_MALLOC_HPP

#include <cstddef>
#include <cstdint>
#include <mutex>

/**
 * @brief Custom memory allocator implementation
 * 
 * This class provides a custom malloc/free implementation with
 * memory pool management, fragmentation handling, and thread safety.
 */
class MyMalloc {
private:
    /**
     * @brief Metadata for memory blocks
     */
    struct Block {
        size_t size;
        bool is_free;
        Block* next;
        uint32_t magic;  // For corruption detection

        Block(size_t s) : size(s), is_free(false), next(nullptr), magic(0xDEADBEEF) {}
    };

    static const size_t MEMORY_POOL_SIZE = 1024 * 1024; // 1MB
    static const uint32_t MAGIC_NUMBER = 0xDEADBEEF;
    static const size_t MIN_BLOCK_SIZE = 16;

    uint8_t memory_pool[MEMORY_POOL_SIZE];
    uint8_t* current_break;
    Block* head;
    mutable std::mutex memory_mutex;

    /**
     * @brief Find a suitable free block
     * @param size Required size
     * @return Pointer to suitable block or nullptr
     */
    Block* findFreeBlock(size_t size);

    /**
     * @brief Split a block if it's larger than needed
     * @param block Block to split
     * @param size Required size
     */
    void splitBlock(Block* block, size_t size);

    /**
     * @brief Coalesce adjacent free blocks
     */
    void coalesceBlocks();

    /**
     * @brief Validate block integrity
     * @param block Block to validate
     * @return true if valid, false otherwise
     */
    bool validateBlock(Block* block) const;

public:
    /**
     * @brief Constructor
     */
    MyMalloc();

    /**
     * @brief Destructor
     */
    ~MyMalloc();

    /**
     * @brief Allocate memory
     * @param size Number of bytes to allocate
     * @return Pointer to allocated memory or nullptr
     */
    void* alloc(size_t size);

    /**
     * @brief Free allocated memory
     * @param ptr Pointer to memory to free
     */
    void free(void* ptr);

    /**
     * @brief Reallocate memory
     * @param ptr Existing pointer (can be nullptr)
     * @param size New size
     * @return Pointer to reallocated memory or nullptr
     */
    void* realloc(void* ptr, size_t size);

    /**
     * @brief Get memory usage statistics
     * @param total_allocated Total bytes allocated
     * @param total_free Total bytes free
     * @param fragmentation_ratio Fragmentation ratio (0.0 to 1.0)
     */
    void getStats(size_t& total_allocated, size_t& total_free, float& fragmentation_ratio) const;

    /**
     * @brief Perform garbage collection (coalesce free blocks)
     */
    void garbageCollect();

    /**
     * @brief Reset allocator (free all memory)
     */
    void reset();
};

#endif // MY_MALLOC_HPP
