cmake_minimum_required(VERSION 4.0)
project(app)

qt_add_executable(financial_calculator calculator.ui calculator_window.ui calculator.hpp calculator.cpp main.cpp)

target_link_libraries(financial_calculator PRIVATE Qt6::Widgets core)
