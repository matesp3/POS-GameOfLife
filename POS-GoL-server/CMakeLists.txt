cmake_minimum_required(VERSION 3.18)
project(POS_GoL_server C)

set(CMAKE_C_STANDARD 11)

add_executable(POS_GoL_server main.c
        buffer.h
        pos_sockets/active_socket.c
        pos_sockets/active_socket.h
        pos_sockets/char_buffer.h
        pos_sockets/char_buffer.c
        pos_sockets/linked_list.h
        pos_sockets/passive_socket.c
        pos_sockets/passive_socket.h
        pole.h
        pole_stringov.h
        pole_stringov.c
        file_handler.h
        file_handler.c)
target_link_libraries(POS_GoL_server pthread)
