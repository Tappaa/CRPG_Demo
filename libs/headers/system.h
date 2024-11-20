#pragma once
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define buffer_size 1024

#define console_width 160
#define console_height 43

struct Point getPrintCenter(char *str, ...);
struct Point getPrintCenterByPos(struct Point startPos, struct Point endPos, char *str, ...);
unsigned long long utf8Strlen(const char* str);
int charCheck (unsigned char c);
int arrayUtf8Strlen(char *str[]);

void setRandomSeed(unsigned int seed);

void skipAbleSleep(DWORD milliSeconds);