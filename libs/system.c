#include "system.h"
#include <windows.h>
#include <stdio.h>

/* basic console functions */

void gotoXY(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

struct Point getConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    struct Point result = { csbi.srWindow.Right - csbi.srWindow.Left, csbi.srWindow.Bottom - csbi.srWindow.Top };
    return result;
}

struct Point getCursorPos() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    struct Point result = { csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y };
    return result;
}

void setCursorVisibility(int isVisible) {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = isVisible;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

/// @Note : in the console, ascii takes up 1 space and UTF-8 takes up 2 spaces.
unsigned __int64 utf8_strlen(const char* str) {
    int length = 0;
    unsigned char c;

    while (*str) {
        c = (unsigned char)*str;

        if (c < 0x80) {
            // ASCII Code (0x00 ~ 0x7F): +1
            length += 1;
            str += 1;
        } else if ((c & 0xE0) == 0xC0) {
            // 2 Bytes UTF-8 Character (110xxxxx): +2
            length += 2;
            str += 2; // move to next character
        } else if ((c & 0xF0) == 0xE0) {
            // 3 Bytes UTF-8 Character (1110xxxx): +2
            length += 2;
            str += 3; // move to next character
        } else if ((c & 0xF8) == 0xF0) {
            // 4 Bytes UTF-8 Character (11110xxx): +2
            length += 2;
            str += 4; // move to next character
        } else {
            // Invalid UTF-8 Character: +1
            str += 1;
        }
    }

    return length;
}

/// @Deprecated : Use setColor(int backColor, int textColor) instead
void setTextColor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

/// @Warning : You must use resetColor() after using this function
void setColor(int backColor, int textColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (backColor << 4) | textColor);
}

void resetColor() {
    setColor(BLACK, WHITE);
}

/* basic console functions */

int getUseableConsoleHeight() {
    struct Point size = getConsoleSize();
    return size.y - 3;
}

/// @Note : Point y is not used (it returns 0)
struct Point getPrintCenter(char *str, ...) {
    struct Point size = getConsoleSize();
    va_list args;
    va_start(args, str);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), str, args);
    int len = (int) utf8_strlen(buffer);

    int x = (size.x - len) / 2;
    struct Point result = { x, 0 };
    va_end(args);
    return result;
}

void printfCenter(int y, char* str, ...) {
    struct Point size = getConsoleSize();
    va_list args;
    va_start(args, str);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), str, args);
    int len = (int) utf8_strlen(buffer);

    int x = (size.x - len) / 2;
    gotoXY(x, y);
    vprintf(str, args);

    va_end(args);
}

void printfXY(int x, int y, char* str, ...) {
    gotoXY(x, y);
    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}

void clearConsoleLines(int start_y, int end_y) {
    struct Point size = getConsoleSize();
    if (end_y == -1 || end_y > size.y) end_y = size.y;
    for (int y = start_y; y <= end_y; y++) {
        gotoXY(0, y);
        for (int x = 0; x <= size.x; x++) {
            printf(" ");
        }
    }
}

int past_level = 0; // 0 : 하얀색 1 : 파란색 3 : 노란색 4 : 빨간색
char past_str[1024] = ""; // 1024 is enough
void printfInInformationBox(int level, char *str, ...) {
    struct Point size = getConsoleSize();
    int y = size.y;

    // clear information box
    clearConsoleLines(y - 1, -1);

    // past string
    if (strcmp(past_str, "") != 0) {
        switch (past_level) {
            case 0:
                setTextColor(WHITE);
                break;
            case 1:
                setTextColor(BLUE);
                break;
            case 2:
                setTextColor(YELLOW);
                break;
            case 3:
                setTextColor(RED);
                break;
            default:
                break;
        }
        printfXY(0, y - 1, past_str);
        setTextColor(WHITE);
    }
    past_level = level;

    switch (level) {
        case 0:
            setTextColor(WHITE);
            break;
        case 1:
            setTextColor(BLUE);
            break;
        case 2:
            setTextColor(YELLOW);
            break;
        case 3:
            setTextColor(RED);
            break;
        default:
            break;
    }
    gotoXY(0, y);
    va_list args;
    va_start(args, str);
    vsnprintf(past_str, sizeof(past_str), str, args);
    vprintf(str, args);
    va_end(args);
    setTextColor(WHITE);
}