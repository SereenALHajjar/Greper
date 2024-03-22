cmake_minimum_required(VERSION 3.15)

project(Greper)

add_library(
    greper_lib
    SHARED
    greper.cpp
)

add_executable(greper main.cpp)

target_include_directories(greper PRIVATE .)

target_link_libraries(
    greper
    PUBLIC
    greper_lib
)