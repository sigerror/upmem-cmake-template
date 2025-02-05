cmake_minimum_required(VERSION 3.16)
project(mylib_tests LANGUAGES C CXX)

add_executable(tests tests.cpp)
target_link_libraries(tests
    PRIVATE mylib::mylib
            Catch2::Catch2WithMain)

include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
catch_discover_tests(tests
    TEST_PREFIX "unittests."
    REPORTER XML
    OUTPUT_DIR .
    OUTPUT_PREFIX "unittests."
    OUTPUT_SUFFIX .xml)
