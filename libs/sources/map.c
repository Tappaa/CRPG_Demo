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
    return mapData;
}

struct Point getPlayerStartLocation(int mapNum) { // TODO ADD Slime, Boss
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
//    printfInInformationBox(0, "[Debug] getMapDataXY: %s", result);
    return result;
}