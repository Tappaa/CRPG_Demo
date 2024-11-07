#pragma once

struct skills {
    char skill_name[20];
    int skill_mp_cost;
    int skill_damage;
    int skill_critical_chance;
    int skill_critical_damage_multiplier;
};

struct enemy_stats {
    char character_symbol[5];
    char ascii_art[20][150];
    int hp;
    int mp;
    int atk;
    int def;

    struct skills skill[5];

    int critical_chance;
    int critical_damage_multiplier;
};

struct enemy_stats getSlimeStats();
struct enemy_stats getBossStats();

int asciiArtLength(struct enemy_stats enemy);

int criticalCheckEnemy(struct enemy_stats enemy);