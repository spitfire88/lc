cmake_minimum_required(VERSION 3.12)
project(MyAlgorithms VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find required packages
find_package(Threads REQUIRED)

# Source files
set(SOURCES
    src/myMemcpy.cpp
    src/myBitSort.cpp
    src/myMalloc.cpp
)

# Create static library
add_library(MyAlgorithms STATIC ${SOURCES})
target_link_libraries(MyAlgorithms Threads::Threads)

# Create test executable
add_executable(testRunner tests/testRunner.cpp)
target_link_libraries(testRunner MyAlgorithms Threads::Threads)

# Enable testing
enable_testing()
add_test(NAME AllTests COMMAND testRunner)

# Create a simple example executable
add_executable(example examples/example.cpp)
target_link_libraries(example MyAlgorithms Threads::Threads)

# Install targets
install(TARGETS MyAlgorithms testRunner example
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install headers
install(DIRECTORY include/ DESTINATION include)

# Print configuration info
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")
