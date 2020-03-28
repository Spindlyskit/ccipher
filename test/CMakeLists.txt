set(CMAKE_C_STANDARD 11)

# Grab Unity testing library
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.5.0
)
FetchContent_GetProperties(unity)
if(NOT unity_POPULATED)
    FetchContent_Populate(unity)
    add_subdirectory(${unity_SOURCE_DIR} ${unity_BINARY_DIR})
endif()

add_executable(ccipher-test ccipher.t.c)

# Link with libccipher and unity
target_link_libraries(ccipher-test PRIVATE libccipher unity)
