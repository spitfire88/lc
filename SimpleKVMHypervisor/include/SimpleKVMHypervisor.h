#ifndef SIMPLE_KVM_HYPERVISOR_H
#define SIMPLE_KVM_HYPERVISOR_H

#include "KVMManager.h"
#include "MemoryManager.h"
#include "VCPUManager.h"
#include "GuestCodeLoader.h"

/**
 * SimpleKVMHypervisor - Version 1.0
 * Main hypervisor class that orchestrates all components
 * 
 * Features in Version 1.0:
 * - Single VCPU support
 * - Basic memory management
 * - Simple guest code loading
 * - Modular architecture for easy extension
 * 
 * Planned for Version 2.0:
 * - Multiple VCPU support
 * - Advanced memory management
 * - Complex guest programs
 * - I/O device emulation
 */
class SimpleKVMHypervisor {
public:
    SimpleKVMHypervisor();
    ~SimpleKVMHypervisor();

    // Main method to create and run a simple VM
    bool createSimpleVM();
    
    // Individual component initialization (for fine-grained control)
    bool initializeComponents();
    bool setupVMMemory(size_t memory_size_mb = 16);
    bool createVirtualCPU();
    bool loadGuestProgram();
    bool executeVM();
    
    // Display comprehensive VM information
    void displayVMInfo();
    
    // Get version information
    static std::string getVersion() { return "1.0"; }
    static std::string getFeatures() { return "Single VCPU, Basic Memory Management, Simple Guest Code Loading"; }

private:
    KVMManager kvm_manager;
    MemoryManager memory_manager;
    VCPUManager vcpu_manager;
    GuestCodeLoader code_loader;
    
    bool initialized;
    
    // Helper methods
    void printBanner();
    void printSeparator(const std::string& title);
};

#endif // SIMPLE_KVM_HYPERVISOR_H
