#include "../allinone.h"

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

void resetColor(HANDLE screen) {
    setColor(screen, BLACK, WHITE);
}

int getAvailableConsoleHeight() {
    struct Point size = getConsoleSize();
    return size.y - 3;
}

int screenBufferIndex = 0;
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
    screenBufferIndex = !screenBufferIndex;
    SetConsoleActiveScreenBuffer(screenBuffer[screenBufferIndex]);
}

HANDLE getCurrentScreenBuffer() {
    return screenBuffer[screenBufferIndex];
}

HANDLE getNextScreenBuffer() {
    return screenBuffer[!screenBufferIndex];
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
    screenBuffer[!screenBufferIndex] = screenBuffer[screenBufferIndex];
}

void clearNextScreenBuffer() {
    COORD coord = { 0, 0 };
    DWORD written;
    FillConsoleOutputCharacter(screenBuffer[screenBufferIndex], ' ', (getConsoleSize().x + 1) *
            getAvailableConsoleHeight(), coord, &written);
    FillConsoleOutputAttribute(screenBuffer[screenBufferIndex], 0, (getConsoleSize().x + 1) *
            getAvailableConsoleHeight(), coord, &written);
}

void destroyScreenBuffer() {
    CloseHandle(screenBuffer[0]);
    CloseHandle(screenBuffer[1]);
}