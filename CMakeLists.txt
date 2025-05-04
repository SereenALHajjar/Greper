cmake_minimum_required(VERSION 3.15)

project(Greper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    greper_lib
    STATIC
    greper.cpp
)

add_executable(greper main.cpp)

target_include_directories(greper PRIVATE .)

target_link_libraries(
    greper
    greper_lib
)