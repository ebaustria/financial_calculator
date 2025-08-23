cmake_minimum_required(VERSION 4.0)
project(financial_calculator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(financial_calculator calculator_window.ui calculator_window.cpp main.cpp)

target_link_libraries(financial_calculator PRIVATE Qt6::Widgets)
