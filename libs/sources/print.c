#include "../manager.h"

void printfCenter(HANDLE screen, int y, char* str, ...) {
    struct Point size = getConsoleSize();
    va_list args;
    va_start(args, str);
    int bufferSize = vsnprintf(NULL, 0, str, args) + 1;
    char *buffer = (char *) malloc(bufferSize);

    vsnprintf(buffer, bufferSize, str, args);
    va_end(args);

    int len = (int) utf8Strlen(buffer);
    int x = (size.x - len) / 2;
    gotoXY(screen, x, y);

    WriteFile(screen, buffer, (DWORD) strlen(buffer), NULL, NULL);
    refreshScreenBuffer();
    free(buffer);
}

void printfXY(HANDLE screen, int x, int y, char* str, ...) {
    gotoXY(screen, x, y);
    va_list args;
    va_start(args, str);
    int bufferSize = vsnprintf(NULL, 0, str, args) + 1;
    char *buffer = (char *) malloc(bufferSize);

    vsnprintf(buffer, bufferSize, str, args);
    va_end(args);

    WriteFile(screen, buffer, (DWORD) strlen(buffer), NULL, NULL);
    refreshScreenBuffer();
    free(buffer);
}

void printf_Buffer(HANDLE screen, char* str, ...) {
    va_list args;
    va_start(args, str);
    int bufferSize = vsnprintf(NULL, 0, str, args) + 1;
    char *buffer = (char *) malloc(bufferSize);

    vsnprintf(buffer, bufferSize, str, args);
    va_end(args);

    WriteFile(screen, buffer, (DWORD) strlen(buffer), NULL, NULL);
    refreshScreenBuffer();
    free(buffer);
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

int past_level = 0; // 0 : 하얀색 1 : 파란색 2 : 노란색 3 : 빨간색
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

void printContinueAction(int y) {
    char* str = "Press any key to continue.";
    int len = (int) strlen(str);

    int x = (getConsoleSize().x - len) / 2;

    HANDLE buffer = getCurrentScreenBuffer();
    SetConsoleActiveScreenBuffer(buffer);

    int animation = 0;

    unsigned __int64 start_tick = GetTickCount64();
    unsigned __int64 ticks = 0;
    while (1) {
        ticks = GetTickCount64() - start_tick;

        if (animation == 0 && ticks >= 500 && ticks < 1000) {
            clearConsoleLines(buffer, y, y);
            animation = 1;
        } else if (animation == 1 && ticks >= 1000) {
            resetColor(buffer);
            gotoXY(buffer, x, y);
            WriteFile(buffer, str, len, NULL, NULL);
            start_tick = GetTickCount64();
            animation = 0;
        }

        if (getInput()) {
            refreshScreenBuffer();
//                CloseHandle(buffer);
            break;
        }
    }
}

void printInformationBoxLine(HANDLE screen) {
    struct Point consoleSize = {
            .x = getConsoleSize().x,
            .y = getAvailableConsoleHeight()
    };

        resetColor(screen);
        for (int x = 0; x < consoleSize.x; x++) {
            gotoXY(screen, x, consoleSize.y + 1);
            WriteFile(screen, "─", 3, NULL, NULL);
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