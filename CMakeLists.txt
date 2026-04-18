cmake_minimum_required(VERSION 4.0)
project(financial_calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG c492b4fed1fd7288eacec6fe8d89e37df78d373a)
FetchContent_MakeAvailable(cpr)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Core)
qt_policy(SET QTP0003 NEW)

qt_standard_project_setup()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} lib/json/include)

add_subdirectory(core)
add_subdirectory(app)
add_subdirectory(lib/Catch2)
add_subdirectory(test)
