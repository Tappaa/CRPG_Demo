#include "../manager.h"

/// @Note : Point y is not used (it returns 0)
struct Point getPrintCenter(char *str, ...) {
    struct Point size = getConsoleSize();
    va_list args;
    va_start(args, str);

    char buffer[buffer_size];
    vsnprintf(buffer, sizeof(buffer), str, args);
    int len = (int) utf8Strlen(buffer);

    int x = (size.x - len) / 2;
    struct Point result = { x, 0 };
    va_end(args);
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

void setRandomSeed(unsigned int seed) {
    srand(seed);
}