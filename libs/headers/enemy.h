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
    int mp;
    int atk;
    int def;

    int skill_count;
    struct skills skill[5];

    int critical_chance;
    int critical_damage_multiplier;

    int color;
};

struct enemy_stats getSlimeStats();
struct enemy_stats getBossStats();

int asciiArtLength(struct enemy_stats enemy);

int criticalCheckEnemy(struct enemy_stats enemy);