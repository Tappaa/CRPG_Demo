#include "../manager.h"

COORD console_buffer_size = { 160, 43 };
SMALL_RECT console_windows_size = { 0, 0, 159, 42 };

/// @Note : Point y is not used (it returns 0)
struct Point getPrintCenter(char *str, ...) {
    struct Point size = getConsoleSize();
    va_list args;
    va_start(args, str);
    int bufferSize = vsnprintf(NULL, 0, str, args) + 1;
    char *buffer = (char *) malloc(bufferSize);

    vsnprintf(buffer, bufferSize, str, args);
    va_end(args);

    int len = (int) utf8Strlen(buffer);
    int x = (size.x - len) / 2 + 1;
    struct Point result = { x, 0 };
    free(buffer);

    return result;
}

struct Point getPrintCenterByPos(struct Point startPos, struct Point endPos, char *str, ...) {
    va_list args;
    va_start(args, str);
    int bufferSize = vsnprintf(NULL, 0, str, args) + 1;
    char *buffer = (char *) malloc(bufferSize);

    vsnprintf(buffer, bufferSize, str, args);
    va_end(args);

    int len = (int) utf8Strlen(buffer);
    int x = (startPos.x + endPos.x - len) / 2 + 1;
    int y = (startPos.y + endPos.y) / 2;
    struct Point result = { x, y };
    free(buffer);

    return result;
}

/// @Note : in the console, ascii takes up 1 space and UTF-8 takes up 2 spaces.
unsigned __int64 utf8Strlen(const char* str) {
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

int charCheck (unsigned char c) {
    if ((c & 0x80) == 0x00) return 1; // 1-byte ASCII
    else if ((c & 0xE0) == 0xC0) return 2; // 2-byte sequence
    else if ((c & 0xF0) == 0xE0) return 3; // 3-byte sequence
    else if ((c & 0xF8) == 0xF0) return 4; // 4-byte sequence
    return -1; // Invalid UTF-8 byte
}

int arrayUtf8Strlen(char *str[]) {
    int size = 0;
    while (str[size] != NULL) {
        size++;
    }

//    printfInInformationBox(0, "[Debug] size: %d", size);
    int total_length = 0;
    for (int i = 0; i < size; i++) {
        total_length += (int) utf8Strlen(str[i]);
    }
    return total_length;
}

void setRandomSeed(unsigned int seed) {
    srand(seed);
}

void skipAbleSleep(DWORD milliSeconds) {
    unsigned __int64 start = GetTickCount64();
    unsigned __int64 now = 0;
    while (now < milliSeconds) {
        now = GetTickCount64() - start;
        if (getInput(1)) {
            if (keyData.key == KEY_ENTER) {
                break;
            }
        }

//        printfInInformationBox(0, "[Debug] Sleep: %d", now);
    }
}
