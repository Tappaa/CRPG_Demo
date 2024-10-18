cmake_minimum_required(VERSION 3.29)
project(CRPG_Demo C)

set(CMAKE_C_FLAGS -s)

set(CMAKE_C_STANDARD 11)

add_executable(CRPG_Demo main.c
        events.c
        libs/system.h
        libs/system.c)
