#pragma once
#include "enemy.h"

struct player_stats {
    char character_symbol[5];
    int hp;
    int max_hp;
    int hp_plus_per_level;

    int mp;
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
};

extern struct player_stats player;

void initPlayer(); // initialize player stats
void createPlayer(struct Point pos); // create player
void levelUp(); // level up player
void fightEnemy(struct enemy_stats enemy); // fight enemy
void playerDead(char* reason); // player dead event
int movePlayer(int direction); // move player to direction(0: up, 1: right, 2: down, 3: left). return 1 if player moved, 0 if not
int mapMovePlayer(int mapNum, struct Point pos); // move player to another map

struct Point getPlayerPosition(); // get player relative position
struct Point getPlayerAbsolutePosition(); // get player absolute position

void setPlayerMove(int move); // set player move

int isPlayerDead(); // return 1 if player is dead, 0 if not
int isPlayerFight(); // return 1 if player is fighting, 0 if not
int canPlayerMove(); // return 1 if player can move, 0 if not
int criticalCheck(); // return 1 if critical hit, 0 if not
