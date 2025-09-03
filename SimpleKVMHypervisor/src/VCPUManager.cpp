#include "../include/VCPUManager.h"
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

VCPUManager::VCPUManager() : vcpu_fd(-1), kvm_run(nullptr), mmap_size(0) {
}

VCPUManager::~VCPUManager() {
    cleanup();
}

bool VCPUManager::createVCPU(int kvm_fd, int vm_fd) {
    std::cout << "Creating virtual CPU..." << std::endl;
    
    // Create VCPU
    vcpu_fd = ioctl(vm_fd, KVM_CREATE_VCPU, 0);
    if (vcpu_fd < 0) {
        std::cerr << "Error: Failed to create VCPU" << std::endl;
        return false;
    }
    
    // Get the size of the kvm_run structure
    mmap_size = ioctl(kvm_fd, KVM_GET_VCPU_MMAP_SIZE, 0);
    if (mmap_size <= 0) {
        std::cerr << "Error: Failed to get VCPU mmap size" << std::endl;
        return false;
    }
    
    // Map the kvm_run structure
    kvm_run = (struct kvm_run*)mmap(nullptr, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, vcpu_fd, 0);
    if (kvm_run == MAP_FAILED) {
        std::cerr << "Error: Failed to mmap kvm_run structure" << std::endl;
        return false;
    }
    
    std::cout << "✓ Virtual CPU created (VCPU ID: 0)" << std::endl;
    return true;
}

bool VCPUManager::initializeVCPURegisters() {
    std::cout << "Initializing VCPU registers..." << std::endl;
    
    // Set up basic registers
    struct kvm_regs regs = {};
    regs.rip = 0x1000; // Start execution at address 0x1000
    regs.rflags = 0x2;  // Basic flags
    
    if (ioctl(vcpu_fd, KVM_SET_REGS, &regs) < 0) {
        std::cerr << "Error: Failed to set VCPU registers" << std::endl;
        return false;
    }
    
    // Set up special registers
    struct kvm_sregs sregs = {};
    if (ioctl(vcpu_fd, KVM_GET_SREGS, &sregs) < 0) {
        std::cerr << "Error: Failed to get special registers" << std::endl;
        return false;
    }
    
    // Set up basic segment registers for real mode
    sregs.cs.base = 0;
    sregs.cs.limit = 0xffff;
    sregs.cs.selector = 0;
    
    sregs.ds.base = 0;
    sregs.ds.limit = 0xffff;
    sregs.ds.selector = 0;
    
    sregs.es.base = 0;
    sregs.es.limit = 0xffff;
    sregs.es.selector = 0;
    
    sregs.ss.base = 0;
    sregs.ss.limit = 0xffff;
    sregs.ss.selector = 0;
    
    if (ioctl(vcpu_fd, KVM_SET_SREGS, &sregs) < 0) {
        std::cerr << "Error: Failed to set special registers" << std::endl;
        return false;
    }
    
    std::cout << "✓ VCPU registers initialized" << std::endl;
    return true;
}

bool VCPUManager::runVM() {
    std::cout << "Running VM..." << std::endl;
    
    // Run the VCPU
    int ret = ioctl(vcpu_fd, KVM_RUN, 0);
    if (ret < 0) {
        std::cerr << "Error: Failed to run VCPU" << std::endl;
        return false;
    }
    
    return handleVMExit();
}

bool VCPUManager::handleVMExit() {
    // Check the exit reason
    switch (kvm_run->exit_reason) {
        case KVM_EXIT_HLT:
            std::cout << "✓ VM executed successfully and halted (HLT instruction)" << std::endl;
            return true;
            
        case KVM_EXIT_IO:
            std::cout << "VM exited due to I/O operation" << std::endl;
            std::cout << "  I/O direction: " << (kvm_run->io.direction ? "OUT" : "IN") << std::endl;
            std::cout << "  I/O port: 0x" << std::hex << kvm_run->io.port << std::dec << std::endl;
            return true;
            
        case KVM_EXIT_MMIO:
            std::cout << "VM exited due to MMIO operation" << std::endl;
            std::cout << "  MMIO address: 0x" << std::hex << kvm_run->mmio.phys_addr << std::dec << std::endl;
            return true;
            
        case KVM_EXIT_SHUTDOWN:
            std::cout << "VM shutdown" << std::endl;
            return true;
            
        case KVM_EXIT_FAIL_ENTRY:
            std::cout << "VM entry failed" << std::endl;
            return false;
            
        default:
            std::cout << "VM exited with reason: " << kvm_run->exit_reason << std::endl;
            return true;
    }
}

void VCPUManager::displayVCPUInfo() const {
    std::cout << "Virtual CPUs: 1 (Version 1.0 - Single VCPU)" << std::endl;
    std::cout << "VCPU FD: " << vcpu_fd << std::endl;
    std::cout << "KVM Run Structure Size: " << mmap_size << " bytes" << std::endl;
}

void VCPUManager::cleanup() {
    if (kvm_run && kvm_run != MAP_FAILED) {
        munmap(kvm_run, mmap_size);
        kvm_run = nullptr;
    }
    
    if (vcpu_fd >= 0) {
        close(vcpu_fd);
        vcpu_fd = -1;
    }
    
    mmap_size = 0;
}
