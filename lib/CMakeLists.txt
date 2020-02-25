add_library(libccipher libccipher.c ../include/libccipher/ccipher.h logging.c ../include/libccipher/logging.h scorer.c
        ../include/libccipher/scorer.h)

target_include_directories(libccipher PUBLIC ../include)
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
