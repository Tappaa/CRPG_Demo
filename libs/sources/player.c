#include "../manager.h"

struct Point player_pos = {0, 0 };
int player_map = 0;
struct player_stats player = { };

int player_dead = 0;
int player_fight = 0;
int player_move = 0;

void initPlayer() {
    strcpy(player.character_symbol, "웃");
    player.max_hp = 70;
    player.hp = player.max_hp;
    player.hp_plus_per_level = 10; // max hp = 70 + 10 * 5 = 120

    player.max_mp = 30;
    player.mp = player.max_mp;
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

void createPlayer(struct Point pos) {
    player_pos = pos;
    // todo init world map
    printfXY(getNextScreenBuffer(), pos.x, pos.y, player.character_symbol);
    setPlayerMove(1);
}

void levelUp() {
    if (player.level >= player.max_level) {
        printfInInformationBox(3, "[!] 레벨업을 할 수 없습니다. 최대 레벨에 도달했습니다.");
        return;
    }

    player.max_hp += player.hp_plus_per_level;
    player.hp = player.max_hp;
    player.max_mp += player.mp_plus_per_level;
    player.mp = player.max_mp;

    player.atk += player.atk_plus_per_level;
    player.def += player.def_plus_per_level;
    player.critical_chance += player.critical_plus_per_level;
    player.level++;

    printfInInformationBox(1, "[!] '플레이어'가 레벨업을 했습니다! 현재 레벨: %d", player.level);
}


void fightEnemy(struct enemy_stats enemy) {

}

extern void ExitGame();
void playerDead(char* reason) {
    player_dead = 1;
    printfInInformationBox(3, "[!] '플레이어'가 사망했습니다. 사망 원인: %s", reason);

    Sleep(1000);
    clearScreenBuffer(getCurrentScreenBufferIndex());
    refreshScreenBuffer();

    printContinueAction(31);
    printfInInformationBox(1, "게임을 종료합니다.");
    Sleep(1000);
    ExitGame();
}

int movePlayer(int direction) {
    struct Point tempPos = getPlayerPosition();

    copyScreenBuffer(getCurrentScreenBufferIndex(), getNextScreenBufferIndex());

    int playerCharLength = (int) utf8Strlen(player.character_symbol);
    char blank[playerCharLength + 1];
    for (int i = 0; i < playerCharLength; i++) {
        blank[i] = ' ';
    }

    switch (direction) {
        case 0:
            tempPos.y--;
            break;
        case 1:
            tempPos.x += playerCharLength;
            break;
        case 2:
            tempPos.y++;
            break;
        case 3:
            tempPos.x -= playerCharLength;
            break;
        default:
            return 0;
    }

    int crashed = isCrashed(player_map, (struct Point) { tempPos.x, tempPos.y });
//    printfInInformationBox(0, "[Debug] isCrashed: %d", crashed);
    if (crashed == 5) {
        return 0;
    } else if (crashed == 1) {
        return 0;
    }

    // delete before position
    printfXY(getNextScreenBuffer(), player_pos.x, player_pos.y, blank);
    // move player
    printfXY(getNextScreenBuffer(), tempPos.x, tempPos.y, player.character_symbol);
    switchNextScreenBuffer();

    player_pos = tempPos;

    return 1;
}

int mapMovePlayer(int mapNum, struct Point pos) {

}


struct Point getPlayerPosition() {
    return player_pos;
}

//struct Point getPlayerAbsolutePosition() {
//    return player_absolute_pos;
//}


void setPlayerMove(int move) {
    player_move = move;
}


int isPlayerDead() {
    return player_dead;
}

int isPlayerFight() {
    return player_fight;
}

int canPlayerMove() {
    return player_move;
}

int criticalCheck() {
    int random = (rand() % 100) + 1;
    printfInInformationBox(0, "[Debug] random: %d, player.critical_chance: %d", random, player.critical_chance);

    return random <= player.critical_chance;
}