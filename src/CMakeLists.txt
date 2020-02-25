add_executable(ccipher libccipher.c ../include/libccipher/logging.h ../include/libccipher/scorer.h)

target_link_libraries(ccipher PRIVATE libccipher)
