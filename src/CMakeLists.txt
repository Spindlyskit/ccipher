add_executable(ccipher ccipher.c ../include/libccipher/logging.h ../include/libccipher/scorer.h ../include/libccipher/ciphers/caesar.h)

target_link_libraries(ccipher PRIVATE libccipher)
