#pragma once

char* getMapFromFile(int mapNum);
struct Point getPlayerStartLocation(int mapNum);
char *getMapDataXY(int mapNum, struct Point pos);