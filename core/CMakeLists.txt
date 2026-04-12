cmake_minimum_required(VERSION 4.0)
project(core)

qt_add_library(core calc_helper_functions.hpp chart_series.hpp chart_series.cpp token.hpp token.cpp)

target_link_libraries(core PRIVATE Qt6::Widgets)