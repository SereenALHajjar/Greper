cmake_minimum_required(VERSION 3.15)

project(Greper)

# add_executable(greper greper.cpp main.cpp)
add_executable(greper main.cpp)

target_include_directories(greper PRIVATE .)