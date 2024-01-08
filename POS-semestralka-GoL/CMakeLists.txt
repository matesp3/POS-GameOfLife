cmake_minimum_required(VERSION 3.18)
project(POS_semestralka_GoL C)

set(CMAKE_C_STANDARD 11)

add_executable(POS_semestralka_GoL main.c
        file_manager.h
        svet.h
        file_manager.c
        svet.c
        terminal_manager.h
        terminal_manager.c
        pole_stringov.h
        pole_stringov.c)
