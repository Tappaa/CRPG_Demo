#pragma once
#include <stdio.h>

enum TYPE {
    HORIZONTAL = 1,
    VERTICAL = 2,
    MATRIX = 3
};

void printfCenter(HANDLE screen, int y, char* str, ...);
void printfAreaCenter(HANDLE screen, struct Point start, struct Point end, char* str, ...);
struct Point printfXY(HANDLE screen, int x, int y, char* str, ...); // return : position of the end of the string
void printf_Buffer(HANDLE screen, char* str, ...);
void clearConsoleLines(HANDLE screen, int start_y, int end_y);
void clearConsoleArea(HANDLE screen, struct Point start, struct Point end);
void printfInInformationBox(int level, char *str, ...); // level : 0 : 하얀색 1 : 파란색 2 : 노란색 3 : 빨간색
void refreshInformationBox(); // If the text looks overlapping, use this function.

// extra functions

void printContinueAction(int y);
extern int selectedIndex;
/// @Note : If the size of the char *str[] array is larger than the input value, add NULL to the end of the array.
/// This creates a button in the middle between startPos and endPos. </br>
/// type -> 1 : horizontal 2 : vertical 3 : matrix </br>
/// gap -> gap between strings </br>
/// count -> count of str </br></br>
/// return -> 1 (-1 if reserved screen buffer or error)
/// selectedIndex -> selected index ( -1 if escape key pressed )
int printSelectAction(int type, struct Point startPos, struct Point endPos, int gap, char* str[], int count, int escape, void (*update)(int));
void printInformationBoxLine(HANDLE screen);
void printEdgeLines(HANDLE screen, struct Point pos1, struct Point pos2);