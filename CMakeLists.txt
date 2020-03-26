cmake_minimum_required(VERSION 3.15)
project(ccipher C)

set(CMAKE_C_STANDARD 11)

# Used to install unity for testing
include(FetchContent)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test EXCLUDE_FROM_ALL)

install(DIRECTORY include/libccipher DESTINATION include)
