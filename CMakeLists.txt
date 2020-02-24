cmake_minimum_required(VERSION 3.15)
project(ccipher C)

set(CMAKE_C_STANDARD 99)

# Used to install unity for testing
include(FetchContent)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)
