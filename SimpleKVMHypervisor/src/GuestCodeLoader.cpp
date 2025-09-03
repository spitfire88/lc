#include "../include/GuestCodeLoader.h"
#include <iostream>
#include <fstream>
#include <iomanip>

GuestCodeLoader::GuestCodeLoader() : load_address(0), code_size(0) {
}

GuestCodeLoader::~GuestCodeLoader() {
}

bool GuestCodeLoader::loadSimpleHaltCode(MemoryManager& memory_manager, size_t load_addr) {
    std::cout << "Loading simple halt guest code..." << std::endl;
    
    // Simple x86 assembly code: HLT instruction (halt)
    // This will cause the VM to exit immediately when executed
    std::vector<uint8_t> halt_code = {
        0xf4  // HLT instruction
    };
    
    code_description = "Simple HLT instruction";
    return loadCustomCode(memory_manager, halt_code, load_addr);
}

bool GuestCodeLoader::loadCustomCode(MemoryManager& memory_manager, const std::vector<uint8_t>& code, size_t load_addr) {
    if (code.empty()) {
        std::cerr << "Error: Empty code provided" << std::endl;
        return false;
    }
    
    if (!validateLoadAddress(load_addr, code.size(), memory_manager.getMemorySize())) {
        return false;
    }
    
    // Copy code to guest memory
    if (!memory_manager.copyToGuestMemory(code.data(), code.size(), load_addr)) {
        std::cerr << "Error: Failed to copy code to guest memory" << std::endl;
        return false;
    }
    
    load_address = load_addr;
    code_size = code.size();
    
    std::cout << "✓ Guest code loaded successfully" << std::endl;
    std::cout << "  Code size: " << code_size << " bytes" << std::endl;
    std::cout << "  Load address: 0x" << std::hex << load_address << std::dec << std::endl;
    
    return true;
}

bool GuestCodeLoader::loadCodeFromFile(MemoryManager& memory_manager, const std::string& filename, size_t load_addr) {
    std::cout << "Loading guest code from file: " << filename << std::endl;
    
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file: " << filename << std::endl;
        return false;
    }
    
    // Get file size
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    if (file_size == 0) {
        std::cerr << "Error: File is empty: " << filename << std::endl;
        return false;
    }
    
    // Read file content
    std::vector<uint8_t> code(file_size);
    file.read(reinterpret_cast<char*>(code.data()), file_size);
    file.close();
    
    code_description = "Code loaded from file: " + filename;
    return loadCustomCode(memory_manager, code, load_addr);
}

bool GuestCodeLoader::validateLoadAddress(size_t address, size_t code_size, size_t memory_size) {
    if (address + code_size > memory_size) {
        std::cerr << "Error: Code would exceed memory bounds" << std::endl;
        std::cerr << "  Load address: 0x" << std::hex << address << std::dec << std::endl;
        std::cerr << "  Code size: " << code_size << " bytes" << std::endl;
        std::cerr << "  Memory size: " << memory_size << " bytes" << std::endl;
        return false;
    }
    
    // Ensure we don't overwrite important memory regions (basic check)
    if (address < 0x1000) {
        std::cerr << "Warning: Loading code at address < 0x1000 may interfere with system structures" << std::endl;
    }
    
    return true;
}

void GuestCodeLoader::displayCodeInfo() const {
    std::cout << "=== Guest Code Information ===" << std::endl;
    std::cout << "Description: " << code_description << std::endl;
    std::cout << "Load Address: 0x" << std::hex << load_address << std::dec << std::endl;
    std::cout << "Code Size: " << code_size << " bytes" << std::endl;
    std::cout << "=============================" << std::endl;
}
