#include "../allinone.h"

void printfCenter(HANDLE screen, int y, char* str, ...) {
    struct Point size = getConsoleSize();
    va_list args;
    va_start(args, str);

    char buffer[buffer_size];
    vsnprintf(buffer, sizeof(buffer), str, args);
    va_end(args);

    int len = (int) utf8_strlen(buffer);
    int x = (size.x - len) / 2;
    gotoXY(screen, x, y);

    WriteFile(screen, buffer, (DWORD) strlen(buffer), NULL, NULL);
    refreshScreenBuffer();
}

void printfXY(HANDLE screen, int x, int y, char* str, ...) {
    gotoXY(screen, x, y);
    va_list args;
    va_start(args, str);
    char buffer[buffer_size];
    vsnprintf(buffer, sizeof(buffer), str, args);
    va_end(args);

    WriteFile(screen, buffer, (DWORD) strlen(buffer), NULL, NULL);
    refreshScreenBuffer();
}

void printf_Buffer(HANDLE screen, char* str, ...) {
    va_list args;
    va_start(args, str);
    char buffer[buffer_size];
    vsnprintf(buffer, sizeof(buffer), str, args);
    va_end(args);

    WriteFile(screen, buffer, (DWORD) strlen(buffer), NULL, NULL);
    refreshScreenBuffer();
}

void clearConsoleLines(HANDLE screen, int start_y, int end_y) {
    struct Point size = getConsoleSize();
    if (end_y == -1 || end_y > size.y) end_y = size.y;
    char empty[size.x + 1];
    for (int x = 0; x <= size.x; x++) {
        empty[x] = ' ';
    }
    for (int y = start_y; y <= end_y; y++) {
        gotoXY(screen, 0, y);
        WriteFile(screen, empty, size.x + 1, NULL, NULL);
    }
    refreshScreenBuffer();
}

int past_level = 0; // 0 : 하얀색 1 : 파란색 3 : 노란색 4 : 빨간색
char past_str[1024] = ""; // 1024 is enough
void printfInInformationBox(int level, char *str, ...) {
    struct Point size = getConsoleSize();
    int y = size.y;
    char buffer[buffer_size];

    for (int i = 0; i < getScreenBufferCount(); i++) {
        // clear information box
        clearConsoleLines(getScreenBuffer(i), y - 1, -1);

        // past string
        if (strcmp(past_str, "") != 0) {
            switch (past_level) {
                case 0:
                    setColor(getScreenBuffer(i), BLACK, WHITE);
                    break;
                case 1:
                    setColor(getScreenBuffer(i), BLACK, BLUE);
                    break;
                case 2:
                    setColor(getScreenBuffer(i), BLACK, YELLOW);
                    break;
                case 3:
                    setColor(getScreenBuffer(i), BLACK, RED);
                    break;
                default:
                    break;
            }
            gotoXY(getScreenBuffer(i), 0, y - 1);
            WriteFile(getScreenBuffer(i), past_str, (DWORD) strlen(past_str), NULL, NULL);
            resetColor(getScreenBuffer(i));
        }

        switch (level) {
            case 0:
                setColor(getScreenBuffer(i), BLACK, WHITE);
                break;
            case 1:
                setColor(getScreenBuffer(i), BLACK, BLUE);
                break;
            case 2:
                setColor(getScreenBuffer(i), BLACK, YELLOW);
                break;
            case 3:
                setColor(getScreenBuffer(i), BLACK, RED);
                break;
            default:
                break;
        }
        va_list args;
        va_start(args, str);
        vsnprintf(buffer, sizeof(buffer), str, args);
        va_end(args);
        gotoXY(getScreenBuffer(i), 0, y);
        WriteFile(getScreenBuffer(i), buffer, (DWORD) strlen(buffer), NULL, NULL);
        resetColor(getScreenBuffer(i));
    }

    past_level = level;
    strcpy(past_str, buffer);
    refreshScreenBuffer();
}

// extra functions

/// @Note : if index is -1, it will print on all screens
void printInformationBoxLine(int index) {
    struct Point consoleSize = {
            .x = getConsoleSize().x,
            .y = getAvailableConsoleHeight()
    };

    int size = index;
    if (index < 0 || index >= getScreenBufferCount()) {
        size = getScreenBufferCount();
    }

    for (int i = 0; i <= size; i++) {
        resetColor(getScreenBuffer(i));
        for (int x = 0; x < consoleSize.x; x++) {
            gotoXY(getScreenBuffer(i), x, consoleSize.y + 1);
            WriteFile(getScreenBuffer(i), "─", 3, NULL, NULL);
        }
    }

    refreshScreenBuffer();
}

void printEdgeLines(HANDLE screen, struct Point pos1, struct Point pos2) {
    struct Point length = {
            .x = pos2.x - pos1.x,
            .y = pos2.y - pos1.y
    };

    resetColor(screen);
    // top and bottom edge lines
    for (int x = 0; x < length.x; x++) {
        gotoXY(screen, pos1.x + x, pos1.y + 0);
        WriteFile(screen, "─", 3, NULL, NULL);
        gotoXY(screen, pos1.x + x, pos1.y + length.y);
        WriteFile(screen, "─", 3, NULL, NULL);
    }
    // left and right edge lines
    for (int y = 0; y <= length.y; y++) {
        gotoXY(screen, pos1.x, pos1.y + y);
        WriteFile(screen, "│", 3, NULL, NULL);
        gotoXY(screen, pos1.x + length.x, pos1.y + y);
        WriteFile(screen, "│", 3, NULL, NULL);
    }

    // corners
    gotoXY(screen, pos1.x + 0, pos1.y + 0);
    WriteFile(screen, "┌", 3, NULL, NULL);
    gotoXY(screen, pos1.x + length.x, pos1.y + 0);
    WriteFile(screen, "┐", 3, NULL, NULL);
    gotoXY(screen, pos1.x + 0, pos1.y + length.y);
    WriteFile(screen, "└", 3, NULL, NULL);
    gotoXY(screen, pos1.x + length.x, pos1.y + length.y);
    WriteFile(screen, "┘", 3, NULL, NULL);

    refreshScreenBuffer();
}