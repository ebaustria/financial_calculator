cmake_minimum_required(VERSION 4.0)
project(core)

qt_add_library(core
        shunting_yard.hpp
        chart_context.hpp
        chart_context.cpp
        strategy.hpp
        strategy.cpp
        token.hpp
        token.cpp
)

target_link_libraries(core PUBLIC Qt6::Widgets Qt6::Charts Qt6::Core cpr)