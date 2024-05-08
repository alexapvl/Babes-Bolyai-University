cmake_minimum_required(VERSION 3.22.4)
project(a4-5)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-fprofile-arcs -ftest-coverage -g -Wall --coverage)
add_link_options(-fprofile-arcs -ftest-coverage)

include_directories(.)

add_executable(main
        app/main.cpp
        src/domain/movie.cpp
        src/repository/repository.cpp
        src/service/service.cpp
        src/ui/ui.cpp
        tests/tests.cpp) 