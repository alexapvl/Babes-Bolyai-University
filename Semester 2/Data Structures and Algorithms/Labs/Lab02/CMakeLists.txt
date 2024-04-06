cmake_minimum_required(VERSION 3.22.4)
project(a)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-fprofile-arcs -ftest-coverage -g -Wall --coverage)
add_link_options(-fprofile-arcs -ftest-coverage)

include_directories(.)

add_executable(main
        App.cpp
        Matrix.cpp
        Matrix.h
        ShortTest.cpp
        ShortTest.h
        ExtendedTest.cpp
        ExtendedTest.h) 
