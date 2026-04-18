cmake_minimum_required(VERSION 4.0)
project(core)

qt_add_library(core
        calc_helper_functions.hpp
        algo.hpp
        chart_series.hpp
        chart_series.cpp
        token.hpp
        token.cpp
        currency_conversion.hpp
        currency_conversion.cpp
)

target_link_libraries(core PUBLIC Qt6::Widgets Qt6::Charts Qt6::Core cpr)