cmake_minimum_required(VERSION 3.22.4)
project(SMM_Sorted_MultiMap_DLL)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-g -Wall)

include_directories(.)

add_executable(main
        App.cpp
        SortedMultiMap.cpp
        SMMIterator.cpp
        ShortTest.cpp
        ExtendedTest.cpp)