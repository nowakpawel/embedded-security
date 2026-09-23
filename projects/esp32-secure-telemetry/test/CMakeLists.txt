cmake_minimum_required(VERSION 3.22)

set(EXTRA_COMPONENT_DIRS
    "../components"
)

set(TEST_COMPONENTS
    telemetry
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32_secure_telemetry_tests)