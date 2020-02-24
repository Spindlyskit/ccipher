add_executable(ccipher-bin ccipher.c)

target_link_libraries(ccipher-bin PRIVATE ccipher)
set_target_properties(ccipher-bin
        PROPERTIES OUTPUT_NAME ccipher)
