cmake_minimum_required(VERSION 3.22.4)
project(a6-7)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-g -Wall)

include_directories(.)

add_executable(main
        app/main.cpp
        src/domain/movie.cpp
        src/repository/repository.cpp
        src/repository/fileRepository.cpp
        src/service/service.cpp
        src/ui/ui.cpp) 