# Grab Unity testing library
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.5.0
)
FetchContent_MakeAvailable(unity)

add_executable(ccipher-test ccipher.t.c)

# Link with libccipher and unity
target_link_libraries(ccipher-test PRIVATE libccipher unity)
