#include "../manager.h"

void printfCenter(HANDLE screen, int y, char* str, ...) {
    // check if screen is reserved
    if (isReservedScreenBufferByHANDLE(screen)) return;

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
    // check if screen is reserved
    if (isReservedScreenBufferByHANDLE(screen)) return;

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
    // check if screen is reserved
    if (isReservedScreenBufferByHANDLE(screen)) return;

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
    // check if screen is reserved
    if (isReservedScreenBufferByHANDLE(screen)) return;

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

void clearConsoleArea(HANDLE screen, struct Point start, struct Point end) {
    // check if screen is reserved
    if (isReservedScreenBufferByHANDLE(screen)) return;

    int x = end.x - start.x + 1;
    int y = end.y - start.y + 1;

    char empty[y][x];
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            empty[i][j] = ' ';
        }
    }

    for (int i = 0; i < y; i++) {
        gotoXY(screen, start.x, start.y + i);
        WriteFile(screen, empty[i], x, NULL, NULL);
    }
}

int pastP_level = 0; // 0 : 하얀색 1 : 파란색 2 : 노란색 3 : 빨간색
char pastP_str[buffer_size] = "";

int past_level = 0; // 0 : 하얀색 1 : 파란색 2 : 노란색 3 : 빨간색
char past_str[buffer_size] = "";
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

    pastP_level = past_level;
    strcpy(pastP_str, past_str);

    past_level = level;
    strcpy(past_str, buffer);
    refreshScreenBuffer();
}

void refreshInformationBox() {
    struct Point size = getConsoleSize();
    int y = size.y;

    for (int i = 0; i < getScreenBufferCount(); i++) {
        // clear information box
        clearConsoleLines(getScreenBuffer(i), y - 1, -1);

        // past string
        if (strcmp(pastP_str, "") != 0) {
            switch (pastP_level) {
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
            WriteFile(getScreenBuffer(i), pastP_str, (DWORD) strlen(pastP_str), NULL, NULL);
            resetColor(getScreenBuffer(i));
        }

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
            gotoXY(getScreenBuffer(i), 0, y);
            WriteFile(getScreenBuffer(i), past_str, (DWORD) strlen(past_str), NULL, NULL);
            resetColor(getScreenBuffer(i));
        }
    }
    refreshScreenBuffer();
}

// extra functions

void printContinueAction(int y) {
    char* str = "계속하려면 아무 키나 누르세요.";
    int len = (int) utf8Strlen(str);

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
            WriteFile(buffer, str, (DWORD) strlen(str), NULL, NULL);
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

int selectedIndex = 0;
int printSelectAction(int type, struct Point startPos, struct Point endPos, int gap, char* str[], int count) {
    // check if screen is reserved
    if (isReservedScreenBufferByHANDLE(getCurrentScreenBuffer())) return -1;

    int delay = 0;
    int delay_ticks = 500;

    struct Point clearPosStart;
    struct Point clearPosEnd;

    while (1) {
        if (delay > 0) delay--;

        if (delay == 0 && getInput()) { // select
            if (type == 1) { // horizontal
                if (keyData.key == KEY_LEFT && keyData.isPressed) {
                    selectedIndex--;
                    if (selectedIndex < 0) selectedIndex = count - 1;
                    delay = delay_ticks;
                } else if (keyData.key == KEY_RIGHT && keyData.isPressed) {
                    selectedIndex++;
                    if (selectedIndex >= count) selectedIndex = 0;
                    delay = delay_ticks;
                } else if (keyData.key == KEY_ENTER && keyData.isPressed) {
                    clearConsoleArea(getCurrentScreenBuffer(), clearPosStart, clearPosEnd);
                    return 1;
                }
            } else if (type == 2) { // vertical
                if (keyData.key == KEY_UP && keyData.isPressed) {
                    selectedIndex--;
                    if (selectedIndex < 0) selectedIndex = count - 1;
                    delay = delay_ticks;
                } else if (keyData.key == KEY_DOWN && keyData.isPressed) {
                    selectedIndex++;
                    if (selectedIndex >= count) selectedIndex = 0;
                    delay = delay_ticks;
                } else if (keyData.key == KEY_ENTER && keyData.isPressed) {
                    clearConsoleArea(getCurrentScreenBuffer(), clearPosStart, clearPosEnd);
                    return 1;
                }
            }
        }

        struct Point printPos;

        if (type == 1) { // horizontal
            int len = arrayUtf8Strlen(str) + (gap * (count - 1));
//            printfInInformationBox(0, "[Debug] len: %d", arrayUtf8Strlen(str));
            char* temp = (char *) malloc(len + 1);
            if (temp == NULL) return -1;

            for (int i = 0; i < len; i++) {
                temp[i] = ' ';
            }
            temp[len] = '\0';

            printPos = getPrintCenterByPos(startPos, endPos, temp);
            clearPosStart = printPos;
            clearPosEnd = (struct Point) { printPos.x + len, printPos.y };
            free(temp);

            for (int i = 0; i < count; i++) {
                if (i != 0) {
                    printPos.x += (int) utf8Strlen(str[i]) + gap + 1;
                }

                if (i == selectedIndex) {
                    setColor(getCurrentScreenBuffer(), WHITE, BLACK);
                } else {
                    resetColor(getCurrentScreenBuffer());
                }
                printfXY(getCurrentScreenBuffer(), printPos.x, printPos.y, str[i]);
            }
        } else if (type == 2) { // vertical
            for (int i = 0; i < count; i++) {
                printPos = getPrintCenterByPos(startPos, endPos, str[i]);

                if (i != 0) {
                    printPos.y += gap + 1;
                    clearPosEnd = (struct Point) { printPos.x + (int) utf8Strlen(str[i]), printPos.y };
                } else {
                    clearPosStart = printPos;
                }

                if (i == selectedIndex) {
                    setColor(getCurrentScreenBuffer(), WHITE, BLACK);
                } else {
                    resetColor(getCurrentScreenBuffer());
                }
                printfXY(getCurrentScreenBuffer(), printPos.x, printPos.y, str[i]);
            }
        }
    }
}

void printInformationBoxLine(HANDLE screen) {
    // check if screen is reserved
    if (isReservedScreenBufferByHANDLE(screen)) return;

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
    // check if screen is reserved
    if (isReservedScreenBufferByHANDLE(screen)) return;

    struct Point length = {
            .x = pos2.x - pos1.x,
            .y = pos2.y - pos1.y
    };

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