#pragma once
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define buffer_size 1024

struct Point getPrintCenter(char *str, ...);
unsigned long long utf8Strlen(const char* str);
int charCheck (unsigned char c);

void setRandomSeed(unsigned int seed);