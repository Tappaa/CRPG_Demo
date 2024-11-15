#pragma once

char* getMapFromFile(int mapNum);
struct Point getPlayerStartLocation(int mapNum);
char *getMapDataXY(int mapNum, struct Point pos);

extern int slimeCount;
extern struct Point *slimePos;
extern int bossCount;
extern struct Point *bossPos;
void mapPrint(int mapNum);

int getIndexFromSlimePos(struct Point pos);
int getIndexFromBossPos(struct Point pos);