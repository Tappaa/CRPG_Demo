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

void setTextColor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

/* basic console functions */

int getUseableConsoleHeight() {
    struct Point size = getConsoleSize();
    return size.y - 3;
}

void printfCenter(int y, char* str, ...) {
    struct Point size = getConsoleSize();
    va_list args;
    va_start(args, str);

    int len = vsnprintf(NULL, 0, str, args);
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
char past_str[250] = ""; // 250 is enough
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