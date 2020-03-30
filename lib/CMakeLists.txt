set(CMAKE_C_STANDARD 11)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_library(libccipher SHARED logging.c scorer.c ciphers/caesar.c ciphers/substitution.c string.c ciphers/vigenere.c)

target_include_directories(libccipher PUBLIC ../include)
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})

install(TARGETS libccipher DESTINATION lib)
