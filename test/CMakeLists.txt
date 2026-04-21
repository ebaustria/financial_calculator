cmake_minimum_required(VERSION 4.0)
project(test)

add_executable(tests
        test_fixture.hpp
        test_fixture.cpp
        calculator_test.cpp
        token_test.cpp
        shunting_yard_test.cpp
        chart_series_test.cpp
)

include_directories(
        ${CMAKE_BINARY_DIR}/app/calculator_autogen/include
        ${CMAKE_BINARY_DIR}/app/calculator_app_autogen/include
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain core calculator)