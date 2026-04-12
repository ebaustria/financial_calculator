cmake_minimum_required(VERSION 4.0)
project(test)

add_executable(tests token_test.cpp algo_test.cpp chart_series_test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain core)