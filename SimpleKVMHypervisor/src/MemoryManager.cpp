#include "../include/MemoryManager.h"
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>
#include <iostream>
#include <cstring>

MemoryManager::MemoryManager() : mem(nullptr), mem_size(0) {
}

MemoryManager::~MemoryManager() {
    cleanup();
}

bool MemoryManager::setupMemory(int vm_fd, size_t memory_size_mb) {
    std::cout << "Setting up VM memory..." << std::endl;
    
    mem_size = memory_size_mb * 1024 * 1024; // Convert MB to bytes
    
    // Allocate memory for the guest
    mem = mmap(nullptr, mem_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        std::cerr << "Error: Failed to allocate memory for guest" << std::endl;
        return false;
    }
    
    // Set up memory region for the VM
    struct kvm_userspace_memory_region mem_region = {};
    mem_region.slot = 0;
    mem_region.guest_phys_addr = 0;
    mem_region.memory_size = mem_size;
    mem_region.userspace_addr = (uint64_t)mem;
    
    if (ioctl(vm_fd, KVM_SET_USER_MEMORY_REGION, &mem_region) < 0) {
        std::cerr << "Error: Failed to set memory region" << std::endl;
        return false;
    }
    
    std::cout << "✓ Memory set up: " << memory_size_mb << "MB allocated" << std::endl;
    return true;
}

bool MemoryManager::copyToGuestMemory(const void* data, size_t size, size_t offset) {
    if (!mem) {
        std::cerr << "Error: Memory not initialized" << std::endl;
        return false;
    }
    
    if (offset + size > mem_size) {
        std::cerr << "Error: Copy would exceed allocated memory bounds" << std::endl;
        return false;
    }
    
    memcpy((char*)mem + offset, data, size);
    return true;
}

void MemoryManager::displayMemoryInfo() const {
    std::cout << "Memory Size: " << (mem_size / 1024 / 1024) << " MB" << std::endl;
    std::cout << "Memory Base Address: " << mem << std::endl;
}

void MemoryManager::cleanup() {
    if (mem && mem != MAP_FAILED) {
        munmap(mem, mem_size);
        mem = nullptr;
    }
    mem_size = 0;
}
