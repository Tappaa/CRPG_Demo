#include "../allinone.h"

struct Point zz = {0, 0 };

int isDisabledKeyInputL = 0;
int isDisabledKeyInput() {
    return isDisabledKeyInputL;
}

/// @Note : 0 : enable, 1 : disable
void setDisabledKeyInput(int isDisabled) {
    isDisabledKeyInputL = isDisabled;
}

/* basic console functions */

void gotoXY(HANDLE screen, int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(screen, pos);
}

struct Point getConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(getCurrentScreenBuffer(), &csbi);
    struct Point result = { csbi.srWindow.Right - csbi.srWindow.Left, csbi.srWindow.Bottom - csbi.srWindow.Top };
    return result;
}

struct Point getCursorPos() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(getCurrentScreenBuffer(), &csbi);
    struct Point result = { csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y };
    return result;
}

/// @Warning : You must use resetColor() after using this function
void setColor(HANDLE screen, int backColor, int textColor) {
    SetConsoleTextAttribute(screen, (backColor << 4) | textColor);
}

/// @Note : Background - Black, Text - White
void resetColor(HANDLE screen) {
    setColor(screen, BLACK, WHITE);
}

int getAvailableConsoleHeight() {
    struct Point size = getConsoleSize();
    return size.y - 3;
}

#define screenBufferCount 2
int currentScreenBufferIndex = 0;
HANDLE screenBuffer[screenBufferCount] = { 0 };
void initScreenBuffer(int cursorVisibility) {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = cursorVisibility;

    for (int i = 0; i < screenBufferCount; i++) {
        screenBuffer[i] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER,
                                                    NULL);
        SetConsoleCursorInfo(screenBuffer[i], &cursorInfo);
    }

    SetConsoleActiveScreenBuffer(screenBuffer[0]);
}

int getCurrentScreenBufferIndex() {
    return currentScreenBufferIndex;
}

int getNextScreenBufferIndex() {
    return (getCurrentScreenBufferIndex() + 1) % screenBufferCount;
}

int getScreenBufferCount() {
    return sizeof(screenBuffer) / sizeof(HANDLE);
}

void switchNextScreenBuffer() {
    currentScreenBufferIndex = getNextScreenBufferIndex();
    SetConsoleActiveScreenBuffer(screenBuffer[getNextScreenBufferIndex()]);
}

void switchScreenBuffer(int index) {
    currentScreenBufferIndex = index;
    SetConsoleActiveScreenBuffer(screenBuffer[index]);
}

HANDLE getCurrentScreenBuffer() {
    return screenBuffer[getCurrentScreenBufferIndex()];
}

HANDLE getNextScreenBuffer() {
    return screenBuffer[getNextScreenBufferIndex()];
}

HANDLE getScreenBuffer(int index) {
    return screenBuffer[index];
}

HANDLE* getAllScreens() {
    return screenBuffer;
}

void refreshScreenBuffer() {
    SetConsoleActiveScreenBuffer(getCurrentScreenBuffer());
}

void copyScreenBuffer(int sourceIndex, int destIndex) {
    screenBuffer[destIndex] = screenBuffer[sourceIndex];
}

/// @Note : if you want to clear current screen buffer, after use this function, you must use refreshScreenBuffer()
void clearScreenBuffer(int index) {
    COORD coord = {0, 0};
    DWORD written;
    FillConsoleOutputCharacter(screenBuffer[index], ' ', (getConsoleSize().x + 1) *
                                            getAvailableConsoleHeight(), coord, &written);
    FillConsoleOutputAttribute(screenBuffer[index], 0, (getConsoleSize().x + 1) *
                                          getAvailableConsoleHeight(), coord, &written);

    printInformationBoxLine(screenBuffer[index]);
}

void destroyScreenBuffer(HANDLE* screen) {
    CloseHandle(*screen);
}