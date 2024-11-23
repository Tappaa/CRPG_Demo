#pragma once

struct skills {
    char skill_name[20];
    int skill_mp_cost;
    int skill_damage;
    int skill_heal;
    int minimum_level;
};

struct enemy_stats {
    char enemy_name[20];
    char character_symbol[5];
    char ascii_art[30][150];
    int hp;
    int hp_per_level;

    int mp;
    int mp_per_level;

    int atk;
    int atk_per_level;

    int def;
    int def_per_level;

    int skill_count;
    struct skills skill[5];

    int critical_chance;
    int critical_damage_multiplier;

    int miss_chance;

    int color;
};

struct enemy_stats getSlimeStats();
struct enemy_stats getBossStats();

int asciiArtLength(struct enemy_stats enemy);

int criticalCheckEnemy(struct enemy_stats enemy);
int missCheckEnemy(struct enemy_stats enemy);