set(CMAKE_C_STANDARD 11)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_library(libccipher SHARED libccipher.c ../include/libccipher/ccipher.h logging.c ../include/libccipher/logging.h scorer.c
        ../include/libccipher/scorer.h ciphers/caesar.c)

target_include_directories(libccipher PUBLIC ../include)
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})

install(TARGETS libccipher DESTINATION lib)
