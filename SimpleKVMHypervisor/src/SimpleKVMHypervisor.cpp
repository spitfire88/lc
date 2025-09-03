#include "../include/SimpleKVMHypervisor.h"
#include <iostream>
#include <iomanip>

SimpleKVMHypervisor::SimpleKVMHypervisor() : initialized(false) {
}

SimpleKVMHypervisor::~SimpleKVMHypervisor() {
    // Cleanup is handled by individual component destructors
}

bool SimpleKVMHypervisor::createSimpleVM() {
    printBanner();
    
    printSeparator("Creating Simple KVM Virtual Machine");
    
    if (!initializeComponents()) {
        return false;
    }
    
    if (!setupVMMemory()) {
        return false;
    }
    
    if (!createVirtualCPU()) {
        return false;
    }
    
    if (!loadGuestProgram()) {
        return false;
    }
    
    displayVMInfo();
    
    if (!executeVM()) {
        return false;
    }
    
    std::cout << "✓ Simple VM created and executed successfully!" << std::endl;
    return true;
}

bool SimpleKVMHypervisor::initializeComponents() {
    if (!kvm_manager.checkKVMAvailability()) {
        return false;
    }
    
    if (!kvm_manager.initializeKVM()) {
        return false;
    }
    
    initialized = true;
    return true;
}

bool SimpleKVMHypervisor::setupVMMemory(size_t memory_size_mb) {
    if (!initialized) {
        std::cerr << "Error: KVM not initialized. Call initializeComponents() first." << std::endl;
        return false;
    }
    
    return memory_manager.setupMemory(kvm_manager.getVMFd(), memory_size_mb);
}

bool SimpleKVMHypervisor::createVirtualCPU() {
    if (!initialized) {
        std::cerr << "Error: KVM not initialized. Call initializeComponents() first." << std::endl;
        return false;
    }
    
    if (!vcpu_manager.createVCPU(kvm_manager.getKVMFd(), kvm_manager.getVMFd())) {
        return false;
    }
    
    return vcpu_manager.initializeVCPURegisters();
}

bool SimpleKVMHypervisor::loadGuestProgram() {
    // Load simple halt program for version 1.0
    return code_loader.loadSimpleHaltCode(memory_manager, 0x1000);
}

bool SimpleKVMHypervisor::executeVM() {
    return vcpu_manager.runVM();
}

void SimpleKVMHypervisor::displayVMInfo() {
    printSeparator("VM Information");
    
    std::cout << "Hypervisor Version: " << getVersion() << std::endl;
    std::cout << "Features: " << getFeatures() << std::endl;
    std::cout << std::endl;
    
    memory_manager.displayMemoryInfo();
    vcpu_manager.displayVCPUInfo();
    
    std::cout << "KVM FD: " << kvm_manager.getKVMFd() << std::endl;
    std::cout << "VM FD: " << kvm_manager.getVMFd() << std::endl;
    
    std::cout << std::endl;
    code_loader.displayCodeInfo();
    
    printSeparator("");
}

void SimpleKVMHypervisor::printBanner() {
    std::cout << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "    Simple KVM Hypervisor - Version " << getVersion() << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "A modular KVM-based hypervisor implementation" << std::endl;
    std::cout << std::endl;
    std::cout << "Current Features:" << std::endl;
    std::cout << "- Single VCPU support" << std::endl;
    std::cout << "- Basic memory management (16MB default)" << std::endl;
    std::cout << "- Simple guest code loading" << std::endl;
    std::cout << "- Modular architecture" << std::endl;
    std::cout << std::endl;
    std::cout << "Planned for Version 2.0:" << std::endl;
    std::cout << "- Multiple VCPU support" << std::endl;
    std::cout << "- Advanced memory management" << std::endl;
    std::cout << "- Complex guest programs" << std::endl;
    std::cout << "- I/O device emulation" << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << std::endl;
}

void SimpleKVMHypervisor::printSeparator(const std::string& title) {
    if (title.empty()) {
        std::cout << "================================================" << std::endl;
    } else {
        std::cout << "=== " << title << " ===" << std::endl;
    }
    std::cout << std::endl;
}
