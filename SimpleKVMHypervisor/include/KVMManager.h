#ifndef KVM_MANAGER_H
#define KVM_MANAGER_H

#include <iostream>

/**
 * KVMManager - Version 1.0
 * Handles KVM availability checking and basic KVM initialization
 */
class KVMManager {
public:
    KVMManager();
    ~KVMManager();

    // Check if KVM is available on the system
    bool checkKVMAvailability();
    
    // Initialize KVM and create a VM
    bool initializeKVM();
    
    // Get file descriptors
    int getKVMFd() const { return kvm_fd; }
    int getVMFd() const { return vm_fd; }
    
    // Cleanup resources
    void cleanup();

private:
    int kvm_fd;
    int vm_fd;
};

#endif // KVM_MANAGER_H
