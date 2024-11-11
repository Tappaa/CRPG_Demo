cmake_minimum_required(VERSION 3.29)
project(CRPG_Demo C)

set(CMAKE_C_STANDARD 11)

add_executable(CRPG_Demo main.c events.c libs/manager.h
        libs/headers/system.h libs/sources/system.c
        libs/headers/keyboard.h libs/sources/keyboard.c
        libs/headers/screen.h libs/sources/screen.c
        libs/headers/print.h libs/sources/print.c
        libs/headers/player.h libs/sources/player.c
        libs/headers/enemy.h libs/sources/enemy.c
        libs/headers/map.h libs/sources/map.c

        libs/headers/obstacle.h libs/sources/obstacle.c)