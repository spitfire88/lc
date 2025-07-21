#include <iostream>
#include <cstdint>
#include <bit>

// Method 1: Union approach
bool isLittleEndian_Union() {
    union {
        uint32_t i;
        uint8_t bytes[4];
    } test;
    
    test.i = 0x12345678;
    return test.bytes[0] == 0x78;  // Little endian if LSB comes first
}

// Method 2: Pointer casting
bool isLittleEndian_Pointer() {
    uint32_t value = 0x12345678;
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&value);
    return ptr[0] == 0x78;
}

// Method 3: Bit shifting with char array
bool isLittleEndian_BitShift() {
    uint16_t value = 0x1234;
    char* ptr = reinterpret_cast<char*>(&value);
    return ptr[0] == 0x34;
}

// Method 4: Using std::endian (C++20)
#if __cplusplus >= 202002L
void checkEndianness_Cpp20() {
    if constexpr (std::endian::native == std::endian::little) {
        std::cout << "Little Endian (C++20)\n";
    } else if constexpr (std::endian::native == std::endian::big) {
        std::cout << "Big Endian (C++20)\n";
    } else {
        std::cout << "Mixed Endian (C++20)\n";
    }
}
#endif

// Method 5: Preprocessor macros (compiler/platform specific)
void checkEndianness_Macros() {
    #ifdef __BYTE_ORDER__
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            std::cout << "Little Endian (Macro)\n";
        #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            std::cout << "Big Endian (Macro)\n";
        #else
            std::cout << "Unknown Endian (Macro)\n";
        #endif
    #else
        std::cout << "Endianness macros not available\n";
    #endif
}

// Method 6: Template-based compile-time detection
template<typename T>
constexpr bool isLittleEndian_Template() {
    constexpr T value = T(0x01);
    return reinterpret_cast<const unsigned char*>(&value)[0] == 0x01;
}

// Method 7: Network byte order comparison
bool isLittleEndian_Network() {
    uint16_t host = 0x1234;
    uint16_t network = htons(host);  // Convert to network byte order (big endian)
    return host != network;  // If different, we're little endian
}

int main() {
    std::cout << "=== Endianness Detection Methods ===\n\n";
    
    // Method 1: Union
    std::cout << "Union method: " 
              << (isLittleEndian_Union() ? "Little" : "Big") 
              << " Endian\n";
    
    // Method 2: Pointer casting
    std::cout << "Pointer method: " 
              << (isLittleEndian_Pointer() ? "Little" : "Big") 
              << " Endian\n";
    
    // Method 3: Bit shifting
    std::cout << "Bit shift method: " 
              << (isLittleEndian_BitShift() ? "Little" : "Big") 
              << " Endian\n";
    
    // Method 4: C++20 std::endian
    #if __cplusplus >= 202002L
    checkEndianness_Cpp20();
    #else
    std::cout << "C++20 std::endian not available\n";
    #endif
    
    // Method 5: Preprocessor macros
    checkEndianness_Macros();
    
    // Method 6: Template (compile-time)
    std::cout << "Template method: " 
              << (isLittleEndian_Template<uint16_t>() ? "Little" : "Big") 
              << " Endian\n";
    
    // Method 7: Network byte order
    std::cout << "Network method: " 
              << (isLittleEndian_Network() ? "Little" : "Big") 
              << " Endian\n";
    
    // Demonstration with actual bytes
    std::cout << "\n=== Byte Layout Demonstration ===\n";
    uint32_t demo = 0x12345678;
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&demo);
    
    std::cout << "Value 0x12345678 stored as bytes: ";
    for (int i = 0; i < 4; i++) {
        printf("0x%02X ", bytes[i]);
    }
    std::cout << "\n";
    
    return 0;
}