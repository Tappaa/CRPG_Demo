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

int currentScreenBufferIndex = 0;
HANDLE screenBuffer[2] = { 0 };
void initScreenBuffer(int cursorVisibility) {
    screenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    screenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = cursorVisibility;
    SetConsoleCursorInfo(screenBuffer[0], &cursorInfo);
    SetConsoleCursorInfo(screenBuffer[1], &cursorInfo);

    SetConsoleActiveScreenBuffer(screenBuffer[0]);
}

int getScreenBufferCount() {
    return sizeof(screenBuffer) / sizeof(HANDLE);
}

void switchScreenBuffer() {
    currentScreenBufferIndex = !currentScreenBufferIndex;
    SetConsoleActiveScreenBuffer(screenBuffer[currentScreenBufferIndex]);
}

HANDLE getCurrentScreenBuffer() {
    return screenBuffer[currentScreenBufferIndex];
}

HANDLE getNextScreenBuffer() {
    return screenBuffer[!currentScreenBufferIndex];
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

void copyScreenBuffer() {
    screenBuffer[!currentScreenBufferIndex] = screenBuffer[currentScreenBufferIndex];
}

void clearNextScreenBuffer() {
    COORD coord = { 0, 0 };
    DWORD written;
    FillConsoleOutputCharacter(screenBuffer[currentScreenBufferIndex], ' ', (getConsoleSize().x + 1) *
                                                                            getAvailableConsoleHeight(), coord, &written);
    FillConsoleOutputAttribute(screenBuffer[currentScreenBufferIndex], 0, (getConsoleSize().x + 1) *
                                                                          getAvailableConsoleHeight(), coord, &written);

    for (int i = 0; i < getScreenBufferCount(); i++) {
        printInformationBoxLine(getScreenBuffer(i));
    }
}

void destroyScreenBuffer() {
    CloseHandle(screenBuffer[0]);
    CloseHandle(screenBuffer[1]);
}