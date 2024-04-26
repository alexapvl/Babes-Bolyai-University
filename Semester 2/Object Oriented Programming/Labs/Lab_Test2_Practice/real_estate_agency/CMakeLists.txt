cmake_minimum_required(VERSION 3.22.4)
project(a)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-g -Wall)

include_directories(.)

add_executable(main
main.cpp
domain.cpp
service.cpp
ui.cpp)