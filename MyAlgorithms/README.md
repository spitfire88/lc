# MyAlgorithms Library

A comprehensive C++ library implementing custom algorithms and data structures for educational and performance-critical applications.

## Features

### 🚀 MyMemcpy
- **Basic Copy**: Simple byte-wise memory copying
- **Word-Aligned Copy**: Optimized copying for aligned memory addresses
- **Optimized Copy**: Intelligent copy strategy selection based on size and alignment
- **Overlap Handling**: Safe copying of overlapping memory regions

### 🔢 MyBitSort
- **Bit-Based Sorting**: Sort numbers based on the count of 1s in their binary representation
- **Flexible Ordering**: Support for both ascending and descending order
- **Efficient Algorithm**: Uses merge sort with Brian Kernighan's bit counting algorithm
- **Large Number Support**: Handles 64-bit unsigned integers

### 💾 MyLRUCache
- **Generic Template**: Works with any key-value pair types
- **Thread-Safe**: Uses reader-writer locks for concurrent access
- **LRU Eviction**: Automatically removes least recently used items when capacity is exceeded
- **Smart Pointers**: Memory-safe implementation using std::shared_ptr

### 🧠 MyMalloc
- **Custom Allocator**: Memory pool-based allocation with fragmentation handling
- **Thread-Safe**: Protected by mutexes for multi-threaded environments
- **Memory Statistics**: Provides allocation statistics and fragmentation analysis
- **Reallocation Support**: Efficient realloc implementation
- **Garbage Collection**: Automatic coalescing of adjacent free blocks

### 🔍 MyBinarySearch
- **Multiple Search Types**: Standard search, first/last occurrence, range queries
- **Insertion Points**: Find optimal insertion positions in sorted arrays
- **Floor/Ceil Operations**: Find closest elements above/below target values
- **Rotated Array Support**: Search in rotated sorted arrays
- **Peak Finding**: Locate peak elements in arrays

## Project Structure

```
MyAlgorithms/
├── include/                 # Header files
│   ├── myMemcpy.hpp
│   ├── myBitSort.hpp
│   ├── myLRUCache.hpp
│   ├── myMalloc.hpp
│   └── myBinarySearch.hpp
├── src/                     # Implementation files
│   ├── myMemcpy.cpp
│   ├── myBitSort.cpp
│   ├── myLRUCache.tpp      # Template implementation
│   ├── myMalloc.cpp
│   └── myBinarySearch.tpp  # Template implementation
├── tests/                   # Test suite
│   └── testRunner.cpp
├── examples/                # Usage examples
│   └── example.cpp
├── build/                   # Build artifacts
├── CMakeLists.txt          # CMake configuration
├── Makefile               # Make configuration
└── README.md             # This file
```

## Building the Project

### Using Make

```bash
# Build everything (library, tests, examples)
make all

# Build with debug information
make debug

# Build optimized release version
make release

# Run test suite
make test

# Run examples
make run-example

# Clean build artifacts
make clean
```

### Using CMake

```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run tests
make test
# or
ctest

# Run specific executables
./testRunner
./example
```

## Usage Examples

### MyMemcpy

```cpp
#include "myMemcpy.hpp"

char source[] = "Hello, World!";
char destination[20];

// Basic copy
MyMemcpy::basicCopy(destination, source, strlen(source) + 1);

// Optimized copy (chooses best strategy automatically)
MyMemcpy::optimizedCopy(destination, source, strlen(source) + 1);
```

### MyBitSort

```cpp
#include "myBitSort.hpp"

MyBitSort sorter;
std::vector<uint64_t> numbers = {0xF, 0x3, 0x7, 0x1}; // Different bit counts

// Sort by number of 1-bits (ascending)
sorter.sortByBitCount(numbers, true);

// Count bits in a number
int ones = sorter.countOnes(0xF); // Returns 4
```

### MyLRUCache

```cpp
#include "myLRUCache.hpp"

MyLRUCache<int, std::string> cache(3); // Capacity of 3

cache.put(1, "one");
cache.put(2, "two");
cache.put(3, "three");

std::string value = cache.get(1); // Returns "one" and marks as recently used
cache.put(4, "four"); // Evicts least recently used item
```

### MyMalloc

```cpp
#include "myMalloc.hpp"

MyMalloc allocator;

// Allocate memory
void* ptr = allocator.alloc(100);

// Use the memory
strcpy(static_cast<char*>(ptr), "Hello");

// Reallocate if needed
ptr = allocator.realloc(ptr, 200);

// Free when done
allocator.free(ptr);

// Get statistics
size_t allocated, free_mem;
float fragmentation;
allocator.getStats(allocated, free_mem, fragmentation);
```

### MyBinarySearch

```cpp
#include "myBinarySearch.hpp"

std::vector<int> arr = {1, 3, 5, 7, 9, 11, 13};

// Basic search
int index = MyBinarySearch<int>::search(arr, 7); // Returns 3

// Find insertion point
int pos = MyBinarySearch<int>::findInsertionPoint(arr, 6); // Returns 3

// Find range of duplicates
std::vector<int> dup_arr = {1, 2, 2, 2, 3};
auto range = MyBinarySearch<int>::findRange(dup_arr, 2); // Returns {1, 3}
```

## Performance Characteristics

- **MyMemcpy**: O(n) time complexity, optimized for different alignment scenarios
- **MyBitSort**: O(n log n) time complexity using merge sort
- **MyLRUCache**: O(1) average time for get/put operations
- **MyMalloc**: O(n) worst-case for allocation (first-fit strategy)
- **MyBinarySearch**: O(log n) time complexity for all search operations

## Thread Safety

- **MyLRUCache**: Fully thread-safe using reader-writer locks
- **MyMalloc**: Thread-safe using mutex protection
- **MyMemcpy**: Thread-safe (no shared state)
- **MyBitSort**: Not thread-safe (operates on user-provided containers)
- **MyBinarySearch**: Thread-safe (read-only operations)

## Testing

The project includes a comprehensive test suite that validates:

- Basic functionality of all algorithms
- Edge cases and error conditions
- Thread safety where applicable
- Performance benchmarks
- Memory leak detection

Run tests with:
```bash
make test
# or
./build/testRunner
```

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.12+ (for CMake build)
- POSIX threads library

## Contributing

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure all tests pass
5. Submit a pull request

## License

This project is intended for educational purposes. Feel free to use and modify as needed.

## Future Enhancements

- [ ] SIMD optimizations for MyMemcpy
- [ ] Parallel sorting for MyBitSort
- [ ] Custom hash functions for MyLRUCache
- [ ] Slab allocator for MyMalloc
- [ ] Interpolation search for MyBinarySearch
