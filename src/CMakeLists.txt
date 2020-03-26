set(CMAKE_C_STANDARD 11)

add_executable(ccipher ccipher.c ../include/libccipher/logging.h ../include/libccipher/scorer.h ../include/libccipher/ciphers/caesar.h)

target_link_libraries(ccipher PRIVATE libccipher)

install(TARGETS ccipher DESTINATION bin)

