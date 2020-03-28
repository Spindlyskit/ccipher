cmake_minimum_required(VERSION 3.12)
project(ccipher C)

set(CMAKE_C_STANDARD 11)

# Used to install unity for testing
include(FetchContent)

add_subdirectory(lib)
add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

# Only build tests if we are the top-level project
# Allows this to be used by super projects with `add_subdirectory`
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

install(DIRECTORY include/libccipher DESTINATION include)
