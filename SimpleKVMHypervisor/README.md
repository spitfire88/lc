# Simple KVM Hypervisor - Version 1.0

A modular KVM-based hypervisor implementation in C++ that demonstrates the fundamental building blocks of virtualization technology.

## Overview

This project provides a clean, modular architecture for creating and managing virtual machines using the KVM (Kernel-based Virtual Machine) infrastructure on Linux. The design prioritizes maintainability and extensibility, making it easy to add new features in future versions.

## Features (Version 1.0)

- ✅ **KVM Availability Checking**: Verifies KVM support and API compatibility
- ✅ **Single VCPU Support**: Creates and manages one virtual CPU
- ✅ **Memory Management**: Allocates and maps guest memory (16MB default)
- ✅ **Guest Code Loading**: Loads simple programs into guest memory
- ✅ **Modular Architecture**: Clean separation of concerns across multiple files
- ✅ **Comprehensive Error Handling**: Detailed error reporting and resource cleanup

## Architecture

The hypervisor is built using a modular design with the following components:

```
SimpleKVMHypervisor/
├── include/
│   ├── KVMManager.h           # KVM initialization and VM creation
│   ├── MemoryManager.h        # Guest memory allocation and management
│   ├── VCPUManager.h          # Virtual CPU creation and execution
│   ├── GuestCodeLoader.h      # Guest program loading utilities
│   └── SimpleKVMHypervisor.h  # Main orchestrator class
├── src/
│   ├── KVMManager.cpp
│   ├── MemoryManager.cpp
│   ├── VCPUManager.cpp
│   ├── GuestCodeLoader.cpp
│   └── SimpleKVMHypervisor.cpp
├── main.cpp                   # Application entry point
├── Makefile                   # Build configuration
└── README.md                  # This file
```

### Component Responsibilities

- **KVMManager**: Handles KVM device access, API version checking, and VM creation
- **MemoryManager**: Manages guest physical memory allocation and mapping
- **VCPUManager**: Creates virtual CPUs, sets up registers, and handles VM execution
- **GuestCodeLoader**: Loads guest programs and manages code placement in memory
- **SimpleKVMHypervisor**: Orchestrates all components and provides high-level API

## Prerequisites

### Hardware Requirements
- x86_64 architecture with Intel VT-x or AMD-V support
- Hardware virtualization enabled in BIOS/UEFI
- Minimum 4GB RAM (8GB+ recommended)

### Software Requirements
- Linux kernel 2.6.20+ with KVM support
- KVM kernel modules loaded (`kvm`, `kvm-intel` or `kvm-amd`)
- GCC 7+ or Clang 5+ with C++17 support
- Make build system

### Verification
```bash
# Check if KVM is available
kvm-ok

# Verify KVM modules are loaded
lsmod | grep kvm

# Check /dev/kvm permissions
ls -l /dev/kvm
```

## Building and Running

### Quick Start
```bash
# Clone or copy the project
cd SimpleKVMHypervisor

# Build the hypervisor
make

# Run the demonstration
make run
```

### Build Options
```bash
# Standard build
make all

# Debug build with symbols
make debug

# Clean build artifacts
make clean

# Show project structure
make structure

# Show version information
make version

# Install system-wide (requires sudo)
make install
```

### Manual Build
```bash
# Create object directory
mkdir -p obj

# Compile source files
g++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude -c src/*.cpp

# Move objects to obj directory
mv *.o obj/

# Link and create executable
g++ -std=c++17 -Wall -Wextra -O2 -g -Iinclude obj/*.o main.cpp -o simple_kvm_hypervisor

# Run
./simple_kvm_hypervisor
```

## Usage Examples

### Basic Usage
```cpp
#include "include/SimpleKVMHypervisor.h"

int main() {
    SimpleKVMHypervisor hypervisor;
    
    if (hypervisor.createSimpleVM()) {
        std::cout << "Success!" << std::endl;
    }
    
    return 0;
}
```

### Advanced Usage (Fine-grained Control)
```cpp
SimpleKVMHypervisor hypervisor;

// Initialize components step by step
if (!hypervisor.initializeComponents()) return 1;
if (!hypervisor.setupVMMemory(32)) return 1;  // 32MB memory
if (!hypervisor.createVirtualCPU()) return 1;
if (!hypervisor.loadGuestProgram()) return 1;

hypervisor.displayVMInfo();

if (!hypervisor.executeVM()) return 1;
```

## Planned Features (Version 2.0)

- 🔄 **Multiple VCPU Support**: Create and manage multiple virtual CPUs
- 🔄 **Advanced Memory Management**: Dynamic allocation, memory ballooning
- 🔄 **Complex Guest Programs**: Support for more sophisticated guest code
- 🔄 **I/O Device Emulation**: Virtual devices and device passthrough
- 🔄 **Networking**: Virtual network interfaces and bridging
- 🔄 **Storage**: Virtual block devices and storage backends
- 🔄 **Live Migration**: VM state save/restore and migration
- 🔄 **Performance Monitoring**: Resource usage tracking and metrics

## Troubleshooting

### Common Issues

1. **Permission Denied on /dev/kvm**
   ```bash
   # Add user to kvm group
   sudo usermod -a -G kvm $USER
   # Log out and back in, or:
   newgrp kvm
   ```

2. **KVM Not Available**
   ```bash
   # Check hardware virtualization
   egrep -c '(vmx|svm)' /proc/cpuinfo
   
   # Load KVM modules
   sudo modprobe kvm
   sudo modprobe kvm_intel  # or kvm_amd
   ```

3. **Compilation Errors**
   ```bash
   # Install build dependencies (Ubuntu/Debian)
   sudo apt-get install build-essential linux-headers-$(uname -r)
   
   # Install KVM development headers
   sudo apt-get install libkvm-dev
   ```

### Debug Mode

Build with debug symbols for troubleshooting:
```bash
make debug
gdb ./simple_kvm_hypervisor
```

## Contributing

This project is designed for educational and experimental purposes. To contribute:

1. Follow the existing modular architecture
2. Add comprehensive error handling
3. Include appropriate documentation
4. Test on multiple Linux distributions
5. Maintain backward compatibility where possible

## Performance Notes

Version 1.0 focuses on functionality and code clarity rather than optimization. Performance improvements are planned for future versions:

- Zero-copy memory operations
- Efficient VM exit handling
- NUMA-aware memory allocation
- Lock-free data structures

## Security Considerations

This is an educational implementation. For production use, consider:

- Input validation and sanitization
- Resource limit enforcement
- Privilege separation
- Secure memory management
- Audit logging

## License

This project is provided for educational purposes. See individual files for specific licensing terms.

## References

- [KVM API Documentation](https://www.kernel.org/doc/Documentation/virtual/kvm/api.txt)
- [Intel Virtualization Technology](https://software.intel.com/content/www/us/en/develop/articles/intel-virtualization-technology.html)
- [AMD Virtualization](https://developer.amd.com/resources/developer-guides-manuals/)
- [Linux KVM](https://www.linux-kvm.org/)

---

**Simple KVM Hypervisor v1.0** - A foundation for understanding virtualization technology.
