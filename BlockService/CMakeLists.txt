cmake_minimum_required(VERSION 3.10)
project(BlockService)

set(CMAKE_CXX_STANDARD 17)

add_library(BlockService SHARED BlockService.cpp)
target_include_directories(BlockService PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
