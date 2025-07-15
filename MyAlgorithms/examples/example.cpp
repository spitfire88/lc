#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <cstring>

#include "../include/myMemcpy.hpp"
#include "../include/myBitSort.hpp"
#include "../include/myLRUCache.hpp"
#include "../include/myMalloc.hpp"
#include "../include/myBinarySearch.hpp"

void demonstrateMyMemcpy() {
    std::cout << "\n=== MyMemcpy Demo ===" << std::endl;
    
    // Example 1: Basic string copy
    const char* source = "Hello, MyMemcpy!";
    char destination[50] = {0};
    
    MyMemcpy::basicCopy(destination, source, strlen(source) + 1);
    std::cout << "Copied string: " << destination << std::endl;
    
    // Example 2: Array copy with optimization
    std::vector<int> srcArray = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> destArray(10);
    
    MyMemcpy::optimizedCopy(destArray.data(), srcArray.data(), 
                           srcArray.size() * sizeof(int));
    
    std::cout << "Copied array: ";
    for (int val : destArray) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void demonstrateMyBitSort() {
    std::cout << "\n=== MyBitSort Demo ===" << std::endl;
    
    MyBitSort sorter;
    
    // Example: Sort numbers by bit count
    std::vector<uint64_t> numbers = {
        0b1111,      // 4 bits
        0b101,       // 2 bits  
        0b11111111,  // 8 bits
        0b1,         // 1 bit
        0b111,       // 3 bits
        0b1010101    // 4 bits
    };
    
    std::cout << "Original numbers (binary representation):" << std::endl;
    for (uint64_t num : numbers) {
        std::cout << std::bitset<8>(num) << " (count: " 
                  << sorter.countOnes(num) << ")" << std::endl;
    }
    
    sorter.sortByBitCount(numbers, true);
    
    std::cout << "\nSorted by bit count (ascending):" << std::endl;
    for (uint64_t num : numbers) {
        std::cout << std::bitset<8>(num) << " (count: " 
                  << sorter.countOnes(num) << ")" << std::endl;
    }
}

void demonstrateMyLRUCache() {
    std::cout << "\n=== MyLRUCache Demo ===" << std::endl;
    
    MyLRUCache<std::string, int> cache(3);  // Capacity of 3
    
    // Add some entries
    cache.put("apple", 100);
    cache.put("banana", 200);
    cache.put("cherry", 300);
    
    std::cout << "Added: apple=100, banana=200, cherry=300" << std::endl;
    cache.dump();
    
    // Access apple (moves it to most recent)
    std::cout << "\nAccessing 'apple': " << cache.get("apple") << std::endl;
    cache.dump();
    
    // Add new entry (should evict least recently used)
    cache.put("date", 400);
    std::cout << "\nAdded date=400 (should evict 'banana'):" << std::endl;
    cache.dump();
    
    // Try to access evicted item
    int value;
    bool found = cache.get("banana", value);
    std::cout << "\nTrying to access 'banana': " 
              << (found ? "Found" : "Not found") << std::endl;
}

void demonstrateMyMalloc() {
    std::cout << "\n=== MyMalloc Demo ===" << std::endl;
    
    MyMalloc allocator;
    
    // Allocate some memory
    void* ptr1 = allocator.alloc(100);
    void* ptr2 = allocator.alloc(200);
    void* ptr3 = allocator.alloc(150);
    
    std::cout << "Allocated 3 blocks: " 
              << (ptr1 ? "✓" : "✗") << " "
              << (ptr2 ? "✓" : "✗") << " " 
              << (ptr3 ? "✓" : "✗") << std::endl;
    
    // Use the allocated memory
    if (ptr1) {
        strcpy(static_cast<char*>(ptr1), "Hello from MyMalloc!");
        std::cout << "Stored in ptr1: " << static_cast<char*>(ptr1) << std::endl;
    }
    
    // Get memory statistics
    size_t allocated, free_mem;
    float fragmentation;
    allocator.getStats(allocated, free_mem, fragmentation);
    std::cout << "Memory stats - Allocated: " << allocated 
              << " bytes, Free: " << free_mem 
              << " bytes, Fragmentation: " << fragmentation << "%" << std::endl;
    
    // Free some memory
    allocator.free(ptr2);
    std::cout << "Freed ptr2" << std::endl;
    
    // Reallocate
    ptr1 = allocator.realloc(ptr1, 300);
    std::cout << "Reallocated ptr1 to 300 bytes: " 
              << (ptr1 ? "Success" : "Failed") << std::endl;
    
    // Cleanup
    allocator.free(ptr1);
    allocator.free(ptr3);
}

void demonstrateMyBinarySearch() {
    std::cout << "\n=== MyBinarySearch Demo ===" << std::endl;
    
    std::vector<int> sortedArray = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    
    std::cout << "Sorted array: ";
    for (int val : sortedArray) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // Basic search
    int target = 7;
    int index = MyBinarySearch<int>::search(sortedArray, target);
    std::cout << "Search for " << target << ": " 
              << (index != -1 ? "Found at index " + std::to_string(index) : "Not found") 
              << std::endl;
    
    // Find insertion point
    target = 8;
    index = MyBinarySearch<int>::findInsertionPoint(sortedArray, target);
    std::cout << "Insertion point for " << target << ": index " << index << std::endl;
    
    // Find floor and ceil
    target = 8;
    int floor_idx = MyBinarySearch<int>::findFloor(sortedArray, target);
    int ceil_idx = MyBinarySearch<int>::findCeil(sortedArray, target);
    
    std::cout << "For value " << target << ":" << std::endl;
    if (floor_idx != -1) {
        std::cout << "  Floor: " << sortedArray[floor_idx] 
                  << " at index " << floor_idx << std::endl;
    }
    if (ceil_idx != -1) {
        std::cout << "  Ceil: " << sortedArray[ceil_idx] 
                  << " at index " << ceil_idx << std::endl;
    }
    
    // Search in array with duplicates
    std::vector<int> arrayWithDups = {1, 2, 2, 2, 3, 4, 5};
    auto range = MyBinarySearch<int>::findRange(arrayWithDups, 2);
    std::cout << "Range of value 2 in [1,2,2,2,3,4,5]: [" 
              << range.first << ", " << range.second << "]" << std::endl;
}

int main() {
    std::cout << "MyAlgorithms Library Examples" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    try {
        demonstrateMyMemcpy();
        demonstrateMyBitSort();
        demonstrateMyLRUCache();
        demonstrateMyMalloc();
        demonstrateMyBinarySearch();
        
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "All examples completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Example execution failed: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
