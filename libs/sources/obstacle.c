#include "../manager.h"

char player_symbol[5];

char* getPlayerSymbol() {
    strcpy(player_symbol, player.character_symbol);

    return player_symbol;
}

char wall_symbol[5];

char* getWallSymbol() {
    strcpy(wall_symbol, "■");

    return wall_symbol;
}

char move_another_map_symbol[5];

char* getMoveAnotherMapSymbol() {
    strcpy(move_another_map_symbol, "⇨");

    return move_another_map_symbol;
}

char slime_symbol[5];

char* getSlimeSymbol() {
    strcpy(slime_symbol, getSlimeStats().character_symbol);

    return slime_symbol;
}

char boss_symbol[5];

char* getBossSymbol() {
    strcpy(boss_symbol, getBossStats().character_symbol);

    return boss_symbol;
}

int isCrashed(int mapNum, struct Point pos) {
    // wall
//    printfInInformationBox(0, "[Debug] %s %d %d", getMapDataXY(mapNum, pos), pos.x, pos.y);
    if (strcmp(getMapDataXY(mapNum, pos), getWallSymbol()) == 0) {
        return 1;
    }
    // slime
    if (*getMapDataXY(mapNum, pos) == 'S') {
        return 2;
    }
    // boss
    if (*getMapDataXY(mapNum, pos) == 'B') {
        return 3;
    }
    // move another map
    if (strcmp(getMapDataXY(mapNum, pos), getMoveAnotherMapSymbol()) == 0) {
        return 4;
    }
    // max relative position
    if (pos.x < 1 || pos.y <= 0 || pos.x >= getConsoleSize().x - 1 || pos.y > getAvailableConsoleHeight()) {
        return 5;
    }
    return 0;
}
