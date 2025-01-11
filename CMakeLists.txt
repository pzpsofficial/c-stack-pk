cmake_minimum_required(VERSION 3.30)
project(stack C)

set(CMAKE_C_STANDARD 11)

add_executable(stack main.c
        error_handler/error_handler.c
        error_handler/error_handler.h
        stack/stack.c
        stack/stack.h
        app_interface/app_interface.c
        app_interface/app_interface.h
        constants.h
        entities/student/student.c
        entities/student/student.h
        lib/text_input/text_input.c
        lib/text_input/text_input.h
        lib/root_file_path_constructor/root_file_path_constructor.c
        lib/root_file_path_constructor/root_file_path_constructor.h)
