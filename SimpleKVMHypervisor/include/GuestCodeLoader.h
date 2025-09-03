#ifndef GUEST_CODE_LOADER_H
#define GUEST_CODE_LOADER_H

#include "MemoryManager.h"
#include <vector>
#include <string>

/**
 * GuestCodeLoader - Version 1.0
 * Handles loading and managing guest code in VM memory
 * This modular design allows for easy extension with more complex guest programs
 */
class GuestCodeLoader {
public:
    GuestCodeLoader();
    ~GuestCodeLoader();

    // Load simple HLT instruction (basic test)
    bool loadSimpleHaltCode(MemoryManager& memory_manager, size_t load_address = 0x1000);
    
    // Load custom assembly code from byte array
    bool loadCustomCode(MemoryManager& memory_manager, const std::vector<uint8_t>& code, size_t load_address = 0x1000);
    
    // Load code from file (for future versions)
    bool loadCodeFromFile(MemoryManager& memory_manager, const std::string& filename, size_t load_address = 0x1000);
    
    // Get information about loaded code
    size_t getLoadAddress() const { return load_address; }
    size_t getCodeSize() const { return code_size; }
    
    // Display code information
    void displayCodeInfo() const;

private:
    size_t load_address;
    size_t code_size;
    std::string code_description;
    
    // Helper function to validate load address
    bool validateLoadAddress(size_t address, size_t code_size, size_t memory_size);
};

#endif // GUEST_CODE_LOADER_H
