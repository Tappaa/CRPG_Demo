#include "../manager.h"

struct enemy_stats getSlimeStats() {
    struct enemy_stats slime = {
            .character_symbol = "●",
            .ascii_art = {
                    "⠀⠀⠀⠀⠀⠀⠀⠀⠀        ⢀⣠⣤⣤⣤⡴⠶⠶⠶⢶⣦⣤⣤⣄⣀",
                    "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⠶⠟⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠷⣤⣄",
                    "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⢶⣄",
                    "⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠿⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠷⣄",
                    "⠀⠀⠀⠀⠀⠀⠀⣤⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣤",
                    "⠀⠀⠀⠀⠀⢀⣼⠟⠀⠀⠀  ⣤⡀⠀⠀⠀⠀⠀⠀  ⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠻⣷⡄",
                    "⠀⠀⠀⠀⢀⣾⠏⠀⠀⠀  ⣾⣿⡇⠀⠀⠀⠀⠀⠀ ⣾⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⢿⣆",
                    "⠀⠀⠀⢀⣼⠇⠀⠀⠀⠀  ⢻⣿⠇ ⠀⠀⠀⠀⠀⠀⢻⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿",
                    "⠀⠀⠀⣼⡏⠀⠀⠀⠀⠀⠀       ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣇",
                    "⠀⠀⢰⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿",
                    "⠀⠀⣼⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿",
                    "⠀⠀⢿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⡏",
                    "⠀⠀⢸⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⡿",
                    "⠀⠀⠀⠙⠻⣿⣷⣦⣤⣤⣤⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⣤⣶⡿",
                    "⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠛⠛⠛⠛⠛⠛⠛⠻⠿⠿⠿⠷⠶⠶⠶⠶⠶⢿⣿⣿⡿⠿⠿⠿⠿⠟⠛⠛⠛⠋"
            },
            .hp = 20,
            .atk = 10,
            .def = 2
    };

    return slime;
}

struct enemy_stats getBossStats() {
    struct skills test = {
        .skill_name = "테스트 스킬",
        .skill_mp_cost = 10,
        .skill_damage = 20,
        .skill_critical_chance = 10,
        .skill_critical_damage_multiplier = 2
    };

    struct enemy_stats boss = {
            .character_symbol = "◈",
            .hp = 100,
            .mp = 2147483647,
            .atk = 20,
            .def = 10,

            .skill = { test }
    };

    return boss;
}

int asciiArtLength(struct enemy_stats enemy) {
    int length = 0;

    for (int i = 0; i < 20; i++) {
        if (strcmp(enemy.ascii_art[i], "") == 0) {
            break;
        }

        length++;
    }

    return length;
}

int criticalCheckEnemy(struct enemy_stats enemy) {
    int random = (rand() % 100) + 1;

    return random <= enemy.critical_chance;
}