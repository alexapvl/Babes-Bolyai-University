cmake_minimum_required(VERSION 3.22.4)
project(Bag)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-g -Wall)

include_directories(.)

add_executable(main
        App.cpp
        Bag.cpp
        BagIterator.cpp
        ExtendedTest.cpp
        ShortTest.cpp)