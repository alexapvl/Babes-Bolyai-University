cmake_minimum_required(VERSION 3.22.4)
project(a)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(main
        app/main.c
		include/domain/date.h
		include/domain/dynamicArray.h
		include/domain/material.h
		include/domain/operation.h
		include/repository/repository.h
		include/repository/undoRedorepository.h
		include/service/service.h
		include/tests/tests.h
		include/ui/ui.h
		src/domain/date.c
		src/domain/dynamicArray.c
		src/domain/material.c
		src/domain/operation.c
		src/repository/repository.c
		src/repository/undoRedorepository.c
		src/service/service.c
		src/tests/tests.c
		src/ui/ui.c)
target_compile_options(main PRIVATE -g -Wall -O2)
