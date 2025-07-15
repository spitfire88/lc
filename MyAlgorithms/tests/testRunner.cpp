#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <cassert>
#include <iomanip>
#include <cstring>
#include <numeric>
#include <algorithm>

#include "../include/myMemcpy.hpp"
#include "../include/myBitSort.hpp"
#include "../include/myLRUCache.hpp"
#include "../include/myMalloc.hpp"
#include "../include/myBinarySearch.hpp"

class TestRunner {
private:
    static void printTestHeader(const std::string& testName) {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "Testing: " << testName << std::endl;
        std::cout << std::string(50, '=') << std::endl;
    }

    static void printTestResult(const std::string& testCase, bool passed) {
        std::cout << std::setw(40) << std::left << testCase 
                  << (passed ? "✓ PASSED" : "✗ FAILED") << std::endl;
    }

public:
    static void testMyMemcpy() {
        printTestHeader("MyMemcpy");

        // Test 1: Basic copy
        {
            char src[] = "Hello, World!";
            char dest[20] = {0};
            MyMemcpy::basicCopy(dest, src, strlen(src) + 1);
            printTestResult("Basic Copy", strcmp(dest, src) == 0);
        }

        // Test 2: Word-aligned copy
        {
            alignas(8) uint64_t src[] = {0x1234567890ABCDEF, 0xFEDCBA0987654321};
            alignas(8) uint64_t dest[2] = {0};
            MyMemcpy::wordAlignedCopy(dest, src, sizeof(src));
            printTestResult("Word-Aligned Copy", dest[0] == src[0] && dest[1] == src[1]);
        }

        // Test 3: Optimized copy with large data
        {
            std::vector<int> src(1000);
            std::vector<int> dest(1000);
            std::iota(src.begin(), src.end(), 1);
            
            MyMemcpy::optimizedCopy(dest.data(), src.data(), src.size() * sizeof(int));
            printTestResult("Optimized Large Copy", src == dest);
        }

        // Test 4: Alignment check
        {
            alignas(8) char aligned_ptr[16];
            char unaligned_ptr[16];
            printTestResult("Alignment Check", 
                          MyMemcpy::isWordAligned(aligned_ptr) && 
                          !MyMemcpy::isWordAligned(unaligned_ptr + 1));
        }

        // Test 5: Null pointer handling
        {
            char buffer[10];
            void* result = MyMemcpy::basicCopy(nullptr, buffer, 10);
            printTestResult("Null Pointer Handling", result == nullptr);
        }
    }

    static void testMyBitSort() {
        printTestHeader("MyBitSort");

        MyBitSort sorter;

        // Test 1: Basic bit counting
        {
            uint64_t num = 0b1010101; // 4 ones
            printTestResult("Count Ones", sorter.countOnes(num) == 4);
        }

        // Test 2: Sort by bit count (ascending)
        {
            std::vector<uint64_t> numbers = {0xF, 0x1F, 0x3F, 0x1, 0x3, 0x7};
            std::vector<uint64_t> expected = {0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F};
            
            sorter.sortByBitCount(numbers, true);
            printTestResult("Sort Ascending by Bit Count", numbers == expected);
        }

        // Test 3: Sort by bit count (descending)
        {
            std::vector<uint64_t> numbers = {0x1, 0x3, 0x7, 0xF};
            sorter.sortByBitCount(numbers, false);
            
            bool sorted_desc = true;
            for (size_t i = 1; i < numbers.size(); i++) {
                if (sorter.countOnes(numbers[i-1]) < sorter.countOnes(numbers[i])) {
                    sorted_desc = false;
                    break;
                }
            }
            printTestResult("Sort Descending by Bit Count", sorted_desc);
        }

        // Test 4: Empty vector
        {
            std::vector<uint64_t> empty_vec;
            sorter.sort(empty_vec);
            printTestResult("Empty Vector Handling", empty_vec.empty());
        }

        // Test 5: Single element
        {
            std::vector<uint64_t> single = {42};
            sorter.sort(single);
            printTestResult("Single Element", single.size() == 1 && single[0] == 42);
        }
    }

    static void testMyLRUCache() {
        printTestHeader("MyLRUCache");

        // Test 1: Basic put/get operations
        {
            MyLRUCache<int, std::string> cache(3);
            cache.put(1, "one");
            cache.put(2, "two");
            cache.put(3, "three");
            
            printTestResult("Basic Put/Get", 
                          cache.get(1) == "one" && 
                          cache.get(2) == "two" && 
                          cache.get(3) == "three");
        }

        // Test 2: LRU eviction
        {
            MyLRUCache<int, int> cache(2);
            cache.put(1, 10);
            cache.put(2, 20);
            cache.put(3, 30); // Should evict key 1
            
            int value;
            bool found = cache.get(1, value);
            printTestResult("LRU Eviction", !found && cache.get(2) == 20 && cache.get(3) == 30);
        }

        // Test 3: Update existing key
        {
            MyLRUCache<int, int> cache(3);
            cache.put(1, 10);
            cache.put(1, 100); // Update
            
            printTestResult("Update Existing Key", cache.get(1) == 100 && cache.size() == 1);
        }

        // Test 4: Remove operation
        {
            MyLRUCache<int, int> cache(3);
            cache.put(1, 10);
            cache.put(2, 20);
            bool removed = cache.remove(1);
            
            int value;
            bool found = cache.get(1, value);
            printTestResult("Remove Operation", removed && !found && cache.size() == 1);
        }

        // Test 5: Thread safety (basic test)
        {
            MyLRUCache<int, int> cache(100);
            std::vector<std::thread> threads;
            
            // Launch multiple threads doing put operations
            for (int i = 0; i < 10; ++i) {
                threads.emplace_back([&cache, i]() {
                    for (int j = 0; j < 10; ++j) {
                        cache.put(i * 10 + j, (i * 10 + j) * 2);
                    }
                });
            }
            
            for (auto& t : threads) {
                t.join();
            }
            
            printTestResult("Thread Safety Basic", cache.size() == 100);
        }
    }

    static void testMyMalloc() {
        printTestHeader("MyMalloc");

        MyMalloc allocator;

        // Test 1: Basic allocation and free
        {
            void* ptr = allocator.alloc(100);
            bool allocated = (ptr != nullptr);
            allocator.free(ptr);
            printTestResult("Basic Alloc/Free", allocated);
        }

        // Test 2: Multiple allocations
        {
            std::vector<void*> ptrs;
            bool all_allocated = true;
            
            for (int i = 0; i < 10; ++i) {
                void* ptr = allocator.alloc(64);
                if (ptr == nullptr) {
                    all_allocated = false;
                    break;
                }
                ptrs.push_back(ptr);
            }
            
            for (void* ptr : ptrs) {
                allocator.free(ptr);
            }
            
            printTestResult("Multiple Allocations", all_allocated);
        }

        // Test 3: Realloc functionality
        {
            void* ptr = allocator.alloc(50);
            if (ptr) {
                strcpy(static_cast<char*>(ptr), "Hello");
                ptr = allocator.realloc(ptr, 100);
                bool reallocated = (ptr != nullptr) && 
                                 (strcmp(static_cast<char*>(ptr), "Hello") == 0);
                allocator.free(ptr);
                printTestResult("Realloc Functionality", reallocated);
            } else {
                printTestResult("Realloc Functionality", false);
            }
        }

        // Test 4: Zero size allocation
        {
            void* ptr = allocator.alloc(0);
            printTestResult("Zero Size Allocation", ptr == nullptr);
        }

        // Test 5: Memory statistics
        {
            allocator.reset();
            void* ptr1 = allocator.alloc(100);
            void* ptr2 = allocator.alloc(200);
            
            size_t allocated, free_mem;
            float fragmentation;
            allocator.getStats(allocated, free_mem, fragmentation);
            
            allocator.free(ptr1);
            allocator.free(ptr2);
            
            printTestResult("Memory Statistics", allocated > 0);
        }
    }

    static void testMyBinarySearch() {
        printTestHeader("MyBinarySearch");

        // Test 1: Basic search
        {
            std::vector<int> arr = {1, 3, 5, 7, 9, 11, 13};
            int index = MyBinarySearch<int>::search(arr, 7);
            printTestResult("Basic Search", index == 3);
        }

        // Test 2: Search not found
        {
            std::vector<int> arr = {1, 3, 5, 7, 9};
            int index = MyBinarySearch<int>::search(arr, 6);
            printTestResult("Search Not Found", index == -1);
        }

        // Test 3: Find first occurrence
        {
            std::vector<int> arr = {1, 2, 2, 2, 3, 4, 5};
            int index = MyBinarySearch<int>::findFirst(arr, 2);
            printTestResult("Find First Occurrence", index == 1);
        }

        // Test 4: Find last occurrence
        {
            std::vector<int> arr = {1, 2, 2, 2, 3, 4, 5};
            int index = MyBinarySearch<int>::findLast(arr, 2);
            printTestResult("Find Last Occurrence", index == 3);
        }

        // Test 5: Find insertion point
        {
            std::vector<int> arr = {1, 3, 5, 7, 9};
            int index = MyBinarySearch<int>::findInsertionPoint(arr, 6);
            printTestResult("Find Insertion Point", index == 3);
        }

        // Test 6: Find floor
        {
            std::vector<int> arr = {1, 3, 5, 7, 9};
            int index = MyBinarySearch<int>::findFloor(arr, 6);
            printTestResult("Find Floor", index == 2 && arr[index] == 5);
        }

        // Test 7: Find ceil
        {
            std::vector<int> arr = {1, 3, 5, 7, 9};
            int index = MyBinarySearch<int>::findCeil(arr, 6);
            printTestResult("Find Ceil", index == 3 && arr[index] == 7);
        }

        // Test 8: Find range
        {
            std::vector<int> arr = {1, 2, 2, 2, 3, 4, 5};
            auto range = MyBinarySearch<int>::findRange(arr, 2);
            printTestResult("Find Range", range.first == 1 && range.second == 3);
        }

        // Test 9: Search in rotated array
        {
            std::vector<int> arr = {4, 5, 6, 7, 0, 1, 2};
            int index = MyBinarySearch<int>::searchRotated(arr, 0);
            printTestResult("Search Rotated Array", index == 4);
        }

        // Test 10: Find peak
        {
            std::vector<int> arr = {1, 3, 20, 4, 1, 0};
            int index = MyBinarySearch<int>::findPeak(arr);
            printTestResult("Find Peak", index == 2 && arr[index] == 20);
        }
    }

    static void runPerformanceBenchmarks() {
        printTestHeader("Performance Benchmarks");

        const size_t large_size = 1000000;
        
        // Benchmark MyMemcpy
        {
            std::vector<int> src(large_size);
            std::vector<int> dest(large_size);
            std::iota(src.begin(), src.end(), 1);
            
            auto start = std::chrono::high_resolution_clock::now();
            MyMemcpy::optimizedCopy(dest.data(), src.data(), src.size() * sizeof(int));
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "MyMemcpy (1M ints): " << duration.count() << " μs" << std::endl;
        }

        // Benchmark MyBitSort
        {
            std::vector<uint64_t> numbers(10000);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<uint64_t> dis(1, UINT64_MAX);
            
            for (auto& num : numbers) {
                num = dis(gen);
            }
            
            MyBitSort sorter;
            auto start = std::chrono::high_resolution_clock::now();
            sorter.sort(numbers);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "MyBitSort (10K elements): " << duration.count() << " ms" << std::endl;
        }

        // Benchmark MyBinarySearch
        {
            std::vector<int> arr(large_size);
            std::iota(arr.begin(), arr.end(), 1);
            
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 1000; ++i) {
                MyBinarySearch<int>::search(arr, 500000);
            }
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "MyBinarySearch (1000 searches in 1M array): " << duration.count() << " μs" << std::endl;
        }
    }

    static void runAllTests() {
        std::cout << "Starting MyAlgorithms Test Suite" << std::endl;
        std::cout << std::string(60, '=') << std::endl;

        testMyMemcpy();
        testMyBitSort();
        testMyLRUCache();
        testMyMalloc();
        testMyBinarySearch();
        runPerformanceBenchmarks();

        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "All tests completed!" << std::endl;
    }
};

int main() {
    try {
        TestRunner::runAllTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test execution failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
