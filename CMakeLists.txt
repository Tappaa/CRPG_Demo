cmake_minimum_required(VERSION 3.29)
project(CRPG_Demo C)

set(CMAKE_C_FLAGS -s)

set(CMAKE_C_STANDARD 11)

add_executable(CRPG_Demo main.c events.c libs/allinone.h
        libs/headers/system.h libs/sources/system.c
        libs/headers/screen.h libs/sources/screen.c
        libs/headers/print.h libs/sources/print.c
        libs/headers/player.h libs/sources/player.c)