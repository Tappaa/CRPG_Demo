#pragma once
#include <windows.h>

struct Point {
    int x;
    int y;
};

enum ColorType{
    BLACK,  	//0
    darkBLUE,	//1
    darkGreen,	//2
    darkSkyBlue,    //3
    darkRed,  	//4
    darkPurple,	//5
    darkYellow,	//6
    GRAY,		//7
    darkGray,	//8
    BLUE,		//9
    GREEN,		//10
    SkyBlue,	//11
    RED,		//12
    PURPLE,		//13
    YELLOW,		//14
    WHITE		//15
};

extern struct Point zz;

int isDisabledKeyInput();
void setDisabledKeyInput(int isDisabled);

/* basic console functions */
void gotoXY(HANDLE screen, int x, int y);
struct Point getConsoleSize();
struct Point getCursorPos();
void setColor(HANDLE screen, int backgroundColor, int textColor);
void resetColor(HANDLE screen);
int getAvailableConsoleHeight();

void initScreenBuffer(int cursorVisibility);
int getCurrentScreenBufferIndex();
int getNextScreenBufferIndex();
int getScreenBufferCount();
void switchNextScreenBuffer();
HANDLE getCurrentScreenBuffer();
HANDLE getNextScreenBuffer();
void switchScreenBuffer(int index);
void setReservedScreenBuffer(int index, int isReserved);
void setAllReservedScreenBuffer(int isReserved);
int isReservedScreenBufferByHANDLE(HANDLE screen);
int isReservedScreenBufferByIndex(int index);
HANDLE getScreenBuffer(int index);
HANDLE* getAllScreens();
void refreshScreenBuffer();
void copyScreenBufferByHANDLE(HANDLE source, HANDLE dest);
void copyScreenBufferByIndex(int sourceIndex, int destIndex);
void clearScreenBufferByHANDLE(HANDLE screen);
void clearScreenBufferByIndex(int index);
void destroyScreenBuffer(HANDLE* screen);