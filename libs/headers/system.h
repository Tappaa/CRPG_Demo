#pragma once
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define buffer_size 1024

/* in game inputs */
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_A 65
#define KEY_B 66
#define KEY_CHEAT_MODE 92
#define KEY_NULL 0
/* in game inputs */

struct Point getPrintCenter(char *str, ...);
unsigned long long utf8_strlen(const char* str);

void setRandomSeed(unsigned int seed);