cmake_minimum_required(VERSION 3.22.4)
project(a)

set(CMAKE_C_STANDARD 20)

include_directories(.)

add_executable(main
        App.cpp
        Matrix.cpp
        Matrix.h
        ShortTest.cpp
        ShortTest.h
        ExtendedTest.cpp
        ExtendedTest.h
  )
target_compile_options(main PRIVATE -g -Wall)
