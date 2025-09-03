#include "../include/KVMManager.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>
#include <iostream>

KVMManager::KVMManager() : kvm_fd(-1), vm_fd(-1) {
}

KVMManager::~KVMManager() {
    cleanup();
}

bool KVMManager::checkKVMAvailability() {
    std::cout << "Checking KVM availability..." << std::endl;
    
    // Check if /dev/kvm exists and is accessible
    int fd = open("/dev/kvm", O_RDWR);
    if (fd < 0) {
        std::cerr << "Error: Cannot open /dev/kvm. Make sure KVM is installed and you have permissions." << std::endl;
        return false;
    }
    
    // Check KVM API version
    int api_version = ioctl(fd, KVM_GET_API_VERSION, 0);
    if (api_version < 0) {
        std::cerr << "Error: Failed to get KVM API version." << std::endl;
        close(fd);
        return false;
    }
    
    if (api_version != KVM_API_VERSION) {
        std::cerr << "Error: KVM API version mismatch. Expected: " << KVM_API_VERSION 
                  << ", Got: " << api_version << std::endl;
        close(fd);
        return false;
    }
    
    std::cout << "✓ KVM is available (API version: " << api_version << ")" << std::endl;
    close(fd);
    return true;
}

bool KVMManager::initializeKVM() {
    std::cout << "Initializing KVM..." << std::endl;
    
    // Open KVM device
    kvm_fd = open("/dev/kvm", O_RDWR);
    if (kvm_fd < 0) {
        std::cerr << "Error: Failed to open /dev/kvm" << std::endl;
        return false;
    }
    
    // Create VM
    vm_fd = ioctl(kvm_fd, KVM_CREATE_VM, 0);
    if (vm_fd < 0) {
        std::cerr << "Error: Failed to create VM" << std::endl;
        return false;
    }
    
    std::cout << "✓ KVM initialized and VM created" << std::endl;
    return true;
}

void KVMManager::cleanup() {
    if (vm_fd >= 0) {
        close(vm_fd);
        vm_fd = -1;
    }
    
    if (kvm_fd >= 0) {
        close(kvm_fd);
        kvm_fd = -1;
    }
}
