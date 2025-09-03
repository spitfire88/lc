#ifndef VCPU_MANAGER_H
#define VCPU_MANAGER_H

#include <linux/kvm.h>

/**
 * VCPUManager - Version 1.0
 * Handles single Virtual CPU creation and management
 * Note: Version 2.0 will support multiple VCPUs
 */
class VCPUManager {
public:
    VCPUManager();
    ~VCPUManager();

    // Create a single virtual CPU
    bool createVCPU(int kvm_fd, int vm_fd);
    
    // Initialize VCPU registers for basic operation
    bool initializeVCPURegisters();
    
    // Run the VM
    bool runVM();
    
    // Get VCPU file descriptor
    int getVCPUFd() const { return vcpu_fd; }
    
    // Get kvm_run structure
    struct kvm_run* getKVMRun() const { return kvm_run; }
    
    // Display VCPU information
    void displayVCPUInfo() const;
    
    // Cleanup VCPU resources
    void cleanup();

private:
    int vcpu_fd;
    struct kvm_run* kvm_run;
    int mmap_size;
    
    // Handle different VM exit reasons
    bool handleVMExit();
};

#endif // VCPU_MANAGER_H
