cmake_minimum_required(VERSION 4.0)
project(app)

qt_add_library(calculator calculator.ui calculator_window.ui calculator.hpp calculator.cpp)

target_link_libraries(calculator PRIVATE Qt6::Widgets Qt6::Charts)

qt_add_executable(calculator_app main.cpp)

target_link_libraries(calculator_app PRIVATE Qt6::Widgets Qt6::Charts calculator core)
