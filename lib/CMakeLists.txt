add_library(ccipher ccipher.c ../include/ccipher/ccipher.h)

target_include_directories(ccipher PUBLIC ../include)
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
