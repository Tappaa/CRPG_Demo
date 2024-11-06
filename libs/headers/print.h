#include <stdio.h>

void printfCenter(HANDLE screen, int y, char* str, ...);
void printfXY(HANDLE screen, int x, int y, char* str, ...);
void printf_Buffer(HANDLE screen, char* str, ...);
void clearConsoleLines(HANDLE screen, int start_y, int end_y);
void printfInInformationBox(int level, char *str, ...); // level : 0 : 하얀색 1 : 파란색 3 : 노란색 4 : 빨간색

// extra functions

void printInformationBoxLine(int index);
void printEdgeLines(HANDLE screen, struct Point pos1, struct Point pos2);