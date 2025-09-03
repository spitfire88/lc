#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <cstdint>
#include <cstddef>

/**
 * MemoryManager - Version 1.0
 * Handles VM memory allocation and management
 */
class MemoryManager {
public:
    MemoryManager();
    ~MemoryManager();

    // Set up memory for the VM
    bool setupMemory(int vm_fd, size_t memory_size_mb = 16);
    
    // Get memory information
    void* getMemoryPtr() const { return mem; }
    size_t getMemorySize() const { return mem_size; }
    
    // Copy data to guest memory at specific offset
    bool copyToGuestMemory(const void* data, size_t size, size_t offset);
    
    // Display memory information
    void displayMemoryInfo() const;
    
    // Cleanup memory resources
    void cleanup();

private:
    void* mem;
    size_t mem_size;
};

#endif // MEMORY_MANAGER_H
