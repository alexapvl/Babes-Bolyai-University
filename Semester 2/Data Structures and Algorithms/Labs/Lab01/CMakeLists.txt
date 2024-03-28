cmake_minimum_required(VERSION 3.22.4)
project(a)

set(CMAKE_C_STANDARD 20)

add_compile_options(-fprofile-arcs -ftest-coverage)
add_link_options(-fprofile-arcs -ftest-coverage)

include_directories(.)

add_executable(main
        App.cpp
        ExtendedTest.cpp
        ExtendedTest.h
        ShortTest.cpp
        ShortTest.h
        FixedCapBiMap.cpp
        FixedCapBiMap.h
        FixedCapBiMapIterator.cpp
        FixedCapBiMapIterator.h)
target_compile_options(main PRIVATE -g -Wall --coverage)
