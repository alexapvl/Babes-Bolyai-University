cmake_minimum_required(VERSION 3.22.4)
project(Bills)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(main
        main.cpp
        domain.cpp
        repository.cpp
        service.cpp
        ui.cpp)