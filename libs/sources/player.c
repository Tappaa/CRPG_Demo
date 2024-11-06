#include "../allinone.h"

struct player_stats player = { };

void initPlayer() {
    strcpy(player.character_symbol, "웃");
    player.hp = 70;
    player.hp_plus_per_level = 10; // max hp = 70 + 10 * 5 = 120

    player.mp = 30;
    player.mp_plus_per_level = 5; // max mp = 30 + 5 * 5 = 55

    player.atk = 5;
    player.atk_plus_per_level = 3; // max atk = 5 + 3 * 5 = 20

    player.def = 3;
    player.def_plus_per_level = 1; // max def = 3 + 1 * 5 = 8

    player.level = 1;
    player.max_level = 5;

    player.critical_chance = 5;
    player.critical_plus_per_level = 1; // max critical chance = 5 + 1 * 5 = 10
    player.critical_damage_multiplier = 2;
}

void levelUp() {
    if (player.level >= player.max_level) {
        printfInInformationBox(4, "[!] 레벨업을 할 수 없습니다. 최대 레벨에 도달했습니다.");
        return;
    }

    player.hp += player.hp_plus_per_level;
    player.mp += player.mp_plus_per_level;
    player.atk += player.atk_plus_per_level;
    player.def += player.def_plus_per_level;
    player.critical_chance += player.critical_plus_per_level;
    player.level++;
}

