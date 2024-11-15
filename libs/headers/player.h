#pragma once
#include "enemy.h"

struct player_stats {
    char character_symbol[5];
    int max_hp;
    int hp_plus_per_level;

    int max_mp;
    int mp_plus_per_level;

    int atk;
    int atk_plus_per_level;

    int def;
    int def_plus_per_level;

    int level;
    int max_level;

    int critical_chance;
    int critical_plus_per_level;
    int critical_damage_multiplier;

    int skill_count;
    struct skills skill[5];
};

extern struct player_stats player;
extern int *foughtSlimes;
extern int *foughtBosses;

void initPlayer(); // initialize player stats
void createPlayer(int mapNum, struct Point pos); // create player
void levelUp(); // level up player
void fightEnemy(struct enemy_stats enemy); // fight enemy
void playerDead(char* reason); // player dead event
void gameClear(); // game clear event
int movePlayer(int direction); // move player to direction(0: up, 1: right, 2: down, 3: left). return 1 if player moved, 0 if not
int mapMovePlayer(int mapNum, struct Point pos); // move player to another map

struct Point getPlayerPosition(); // get player relative position
int getPlayerMap(); // get player map number

void setPlayerMove(int move); // set player move

int isPlayerDead(); // return 1 if player is dead, 0 if not
int isPlayerFight(); // return 1 if player is fighting, 0 if not
int canPlayerMove(); // return 1 if player can move, 0 if not
int criticalCheck(); // return 1 if critical hit, 0 if not
