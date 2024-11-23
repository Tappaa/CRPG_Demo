#include "../manager.h"

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

struct Point getCursorPos(HANDLE screen) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(screen, &csbi);
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
int reservedScreenBufferIndex[screenBufferCount] = { 0 };
HANDLE screenBuffer[screenBufferCount] = { 0 };
void initScreenBuffer(int cursorVisibility) {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = cursorVisibility;

    for (int i = 0; i < screenBufferCount; i++) {
        screenBuffer[i] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER,
                                                    NULL);
        SetConsoleCursorInfo(screenBuffer[i], &cursorInfo);

        // disable line wrap
        DWORD console_mode;
        GetConsoleMode(screenBuffer[i], &console_mode);
        SetConsoleMode(screenBuffer[i], console_mode & ~ENABLE_WRAP_AT_EOL_OUTPUT);
        SetConsoleScreenBufferSize(screenBuffer[i], (COORD) { console_height, console_width });
        SetConsoleWindowInfo(screenBuffer[i], TRUE, &((SMALL_RECT) { 0, 0, console_width - 1, console_height - 1 }));
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
    if (reservedScreenBufferIndex[getNextScreenBufferIndex()] == 1) {
        reservedScreenBufferIndex[getNextScreenBufferIndex()] = 0;
    }

    currentScreenBufferIndex = getNextScreenBufferIndex();
    SetConsoleActiveScreenBuffer(screenBuffer[getCurrentScreenBufferIndex()]);
//  printfInInformationBox(0, "[Debug] Screen buffer switched to %d", currentScreenBufferIndex);
}

void switchScreenBuffer(int index) {
    if (reservedScreenBufferIndex[index] == 1) {
        reservedScreenBufferIndex[index] = 0;
    }

    currentScreenBufferIndex = index;
    SetConsoleActiveScreenBuffer(screenBuffer[index]);
}

void setReservedScreenBuffer(int index, int isReserved) {
    reservedScreenBufferIndex[index] = isReserved;
}

void setAllReservedScreenBuffer(int isReserved) {
    for (int i = 0; i < screenBufferCount; i++) {
        reservedScreenBufferIndex[i] = isReserved;
    }
}

int isReservedScreenBufferByHANDLE(HANDLE screen) {
    for (int i = 0; i < screenBufferCount; i++) {
        if (screenBuffer[i] == screen) {
            return reservedScreenBufferIndex[i];
        }
    }

    return 0;
}

int isReservedScreenBufferByIndex(int index) {
    return reservedScreenBufferIndex[index];
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

void copyScreenBufferByHANDLE(HANDLE source, HANDLE dest) {
    // if reserved, do not copy
    if (isReservedScreenBufferByHANDLE(dest) == 1) return;

    COORD bufferSize = {(short) getConsoleSize().x, (short) getConsoleSize().y};
    SMALL_RECT readRegion = {0, 0, bufferSize.X, bufferSize.Y};
    COORD bufferCoord = {0, 0};
    CHAR_INFO* charInfoBuffer = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * bufferSize.X * bufferSize.Y);

    if (charInfoBuffer == NULL) {
        // error memory allocation
        return;
    }

    if (!ReadConsoleOutputW(source, charInfoBuffer, bufferSize, bufferCoord, &readRegion)) {
        free(charInfoBuffer);
        return;
    }

    if (!WriteConsoleOutputW(dest, charInfoBuffer, bufferSize, bufferCoord, &readRegion)) {
    }

    free(charInfoBuffer);
}

void copyScreenBufferByIndex(int sourceIndex, int destIndex) {
    // if reserved, do not copy
    if (isReservedScreenBufferByIndex(destIndex) == 1) return;

    COORD bufferSize = {(short) getConsoleSize().x, (short) getConsoleSize().y};
    SMALL_RECT readRegion = {0, 0, bufferSize.X, bufferSize.Y};
    COORD bufferCoord = {0, 0};
    CHAR_INFO* charInfoBuffer = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * bufferSize.X * bufferSize.Y);

    if (charInfoBuffer == NULL) {
        // error memory allocation
        return;
    }

    if (!ReadConsoleOutputW(screenBuffer[sourceIndex], charInfoBuffer, bufferSize, bufferCoord, &readRegion)) {
        free(charInfoBuffer);
        return;
    }

    if (!WriteConsoleOutputW(screenBuffer[destIndex], charInfoBuffer, bufferSize, bufferCoord, &readRegion)) {
    }

    free(charInfoBuffer);
}

void clearScreenBufferByHANDLE(HANDLE screen) {
    // if reserved, do not clear
    if (isReservedScreenBufferByHANDLE(screen) == 1) return;

    COORD coord = {0, 0};
    DWORD written;
    FillConsoleOutputCharacterW(screen, ' ', (getConsoleSize().x + 1) * (getAvailableConsoleHeight() + 1), coord,
                                 &written);
    FillConsoleOutputAttribute(screen, 0, (getConsoleSize().x + 1) * (getAvailableConsoleHeight() + 1), coord,
                               &written);

    printInformationBoxLine(screen);
}

void clearScreenBufferByIndex(int index) {
    // if reserved, do not clear
    if (isReservedScreenBufferByIndex(index) == 1) return;

    COORD coord = {0, 0};
    DWORD written;
    FillConsoleOutputCharacterW(screenBuffer[index], ' ', (getConsoleSize().x + 1) *
                                                          (getAvailableConsoleHeight() + 1), coord, &written);
    FillConsoleOutputAttribute(screenBuffer[index], 0, (getConsoleSize().x + 1) *
                                                       (getAvailableConsoleHeight() + 1), coord, &written);

    printInformationBoxLine(screenBuffer[index]);
}

void destroyScreenBuffer(HANDLE* screen) {
    CloseHandle(*screen);
}