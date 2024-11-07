#pragma once

char* getPlayerSymbol();
char* getWallSymbol();
char* getMoveAnotherMapSymbol();
char* getSlimeSymbol();
char* getBossSymbol();

int isCrashed(int mapNum, struct Point pos); // 1: wall, 2: slime, 3: boss, 4: move another map, 5: max relative position, 0: not crashed