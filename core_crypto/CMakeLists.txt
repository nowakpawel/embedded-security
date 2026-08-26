cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0135 NEW)
project(CoreCrypto VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_STANDARD_EXTENSIONS  OFF)

add_library(core_crypto
            src/crypto_buffer.cpp)

target_include_directories(core_crypto PUBLIC
        message("CURRENT SOURCE DIR: " ${CURRENT_SOURCE_DIR})
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (MSVC)
    target_compile_options(core_crypto PRIVATE /W4)
else ()
    target_compile_options(core_crypto PRIVATE -Wall -Wextra -Wpedantic)
endif ()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)