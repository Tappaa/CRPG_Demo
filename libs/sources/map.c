#include <sys/stat.h>
#include "../manager.h"

char* getMapFromFile(int mapNum) {
    FILE *mapFile;
    char fileName[20];
    struct stat size;

    sprintf(fileName, "maps/%d.map_data", mapNum);
    mapFile = fopen(fileName, "rb");

    if (stat(fileName, &size) == -1) {
//        printfInInformationBox(3, "[Error] map file not found");
    } else {
//        printfInInformationBox(0, "[Debug] map file size: %d", size.st_size);
    }

    if (mapFile == NULL) {
//        printfInInformationBox(3, "[Error] map file not found");
    } else {
//        printfInInformationBox(0, "[Debug] map file found");
    }

    char *mapData = (char *) malloc(size.st_size + 1);
    fread(mapData, size.st_size, 1, mapFile);
    mapData[size.st_size] = '\0';
    fclose(mapFile);
    return mapData;
}

struct Point getPlayerStartLocation(int mapNum) {
    struct Point result = { 0, 0 };
    const char *mapData = getMapFromFile(mapNum);

    while (*mapData != '\0') {
        if (*mapData == '\n') {
            result.y++;
            result.x = 0;
            mapData++;
            continue;
        }
        if (*mapData == 'P') {
            result.x--;
            break;
        }
        result.x++;
        mapData++;
    }

    return result;
}

char* getMapDataXY(int mapNum, struct Point target) {
    int x = 0, y = 0;
    const char *ptr = getMapFromFile(mapNum);
    char *result = "X";

    while (*ptr != '\0') {
        if (*ptr == '\n') {
            y++;
            x = 0;
            ptr++;
            continue;
        }

        int char_len = charCheck((unsigned char)*ptr);
        if (char_len == -1) { // Invalid UTF-8 encoding detected
            return "X";
        }

        if (x == target.x && y == target.y) {
            result = (char *) malloc(char_len + 1);
            for (int i = 0; i < char_len; i++) {
                result[i] = ptr[i];
            }
            result[char_len] = '\0';
            break;
        }

        ptr += char_len;
        if (char_len == 1) {
            x++;
        } else {
            x += 2;
        }
    }
//    printfInInformationBox(0, "[Debug] getMapDataXY: %s, x:%d, y:%d", result, x, y);
    return result;
}

int slimeCount = 0;
struct Point *slimePos = NULL;
int bossCount = 0;
struct Point *bossPos = NULL;
void mapPrint(int mapNum) {
    slimeCount = 0;
    bossCount = 0;


    const char *mapData = getMapFromFile(mapNum);
    struct Point pos = { 0, 0 };

    while (*mapData != '\0' && *mapData != EOF) {
        int char_len = charCheck((unsigned char)*mapData);

        if (*mapData == '\n') {
            pos.y++;
            pos.x = 0;
            mapData++;
            continue;
        }

        char *str = (char *) malloc(char_len + 1);
        for (int i = 0; i < char_len; i++) {
            str[i] = mapData[i];
        }
        str[char_len] = '\0';

        if (*str == '^') {
            setColor(getNextScreenBuffer(), BLACK, GREEN);
            printfXY(getNextScreenBuffer(), pos.x, pos.y, "%c", *mapData);
            resetColor(getNextScreenBuffer());
//            printfInInformationBox(0, "[Debug] ^ %d, %d", pos.x, pos.y);
        } else if (*str == '|') {
            // brown color
            setColor(getNextScreenBuffer(), FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_RED | FOREGROUND_GREEN);
            printfXY(getNextScreenBuffer(), pos.x, pos.y, "%c", *mapData);
            resetColor(getNextScreenBuffer());
//            printfInInformationBox(0, "[Debug] | %d, %d", pos.x, pos.y);
        } else if (*str == 'S') {
            setColor(getNextScreenBuffer(), BLACK, GREEN);
            printfXY(getNextScreenBuffer(), pos.x, pos.y, "%s", getSlimeSymbol());
            resetColor(getNextScreenBuffer());

            slimeCount++;
            slimePos = (struct Point *) realloc(slimePos, slimeCount * sizeof(struct Point));
            slimePos[slimeCount - 1] = pos;
//            printfInInformationBox(0, "[Debug] S %d, %d", pos.x, pos.y);
        } else if (*str == 'B') {
            setColor(getNextScreenBuffer(), BLACK, RED);
            printfXY(getNextScreenBuffer(), pos.x, pos.y, "%s", getBossSymbol());
            resetColor(getNextScreenBuffer());

            bossCount++;
            bossPos = (struct Point *) realloc(bossPos, bossCount * sizeof(struct Point));
            bossPos[bossCount - 1] = pos;
//            printfInInformationBox(0, "[Debug] B %d, %d", pos.x, pos.y);
        } else {
            printfXY(getNextScreenBuffer(), pos.x, pos.y, "%s", str);
//            printfInInformationBox(0, "[Debug] %d, %d", pos.x, pos.y);
        }

        mapData += char_len;
        if (char_len == 1) {
            pos.x++;
        } else {
            pos.x += 2;
        }
    }
//    free((char *) mapData);
}

int getIndexFromSlimePos(struct Point pos) {
    for (int i = 0; i < slimeCount; i++) {
        if (slimePos[i].x == pos.x && slimePos[i].y == pos.y) {
            return i;
        }
    }
    return -1;
}

int getIndexFromBossPos(struct Point pos) {
    for (int i = 0; i < bossCount; i++) {
        if (bossPos[i].x == pos.x && bossPos[i].y == pos.y) {
            return i;
        }
    }
    return -1;
}