add_executable(unit_tests
    tests_crypto_buffer.cpp
)

target_link_libraries(unit_tests PRIVATE
        core_crypto
        gtest_main
)

add_test(NAME unit_tests COMMAND unit_tests)