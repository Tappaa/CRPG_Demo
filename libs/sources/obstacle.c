#include "../manager.h"

char player_symbol[5];

char* getPlayerSymbol() {
    strcpy(player_symbol, player.character_symbol);

    return player_symbol;
}

char wall_symbol[5];

char* getWallSymbol() {
    strcpy(wall_symbol, "■"); // TODO: change this

    return wall_symbol;
}

char move_another_map_symbol[5];

char* getMoveAnotherMapSymbol() {
    strcpy(move_another_map_symbol, "⇨"); // TODO: change this

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

int isCrashed(int mapNum, struct Point pos) { // todo mapNum
    // max relative position
    if (pos.x < 0 || pos.y <= 0 || pos.x >= getConsoleSize().x || pos.y >

    getAvailableConsoleHeight()) {
        return 5;
    }
}
