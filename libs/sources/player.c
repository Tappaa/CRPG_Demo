#include "../manager.h"

 struct Point player_pos = { 0, 0 };
struct Point temp_pos = { 0, 0 };
int player_map = 0;
struct player_stats player = { };

int player_dead = 0;
int player_fight = 0;
int player_move = 0;

int *foughtSlimes = NULL;
int *foughtBosses = NULL;

void initPlayer() {
    strcpy(player.character_symbol, "웃");
    player.max_hp = 70;
    player.hp_plus_per_level = 10; // max hp = 70 + 10 * 5 = 120

    player.max_mp = 30;
    player.mp_plus_per_level = 5; // max mp = 30 + 5 * 5 = 55

    player.atk = 8;
    player.atk_plus_per_level = 2; // max atk = 8 + 2 * 5 = 18

    player.def = 5;
    player.def_plus_per_level = 1; // max def = 5 + 1 * 5 = 10

    player.level = 1;
    player.max_level = 5;

    player.critical_chance = 5;
    player.critical_plus_per_level = 1; // max critical chance = 5 + 1 * 5 = 10
    player.critical_damage_multiplier = 2;
}

void createPlayer(int mapNum, struct Point pos) {
    player_pos = pos;
    player_map = mapNum;

    mapPrint(mapNum);
    setColor(getNextScreenBuffer(), BLACK, SkyBlue);
    printfXY(getNextScreenBuffer(), pos.x, pos.y, player.character_symbol);
    resetColor(getNextScreenBuffer());
    setPlayerMove(1);

    foughtSlimes = (int*) malloc(sizeof(int) * slimeCount);
    foughtBosses = (int*) malloc(sizeof(int) * bossCount);
    for (int i = 0; i < slimeCount; i++) {
        foughtSlimes[i] = 0;
    }

    for (int i = 0; i < bossCount; i++) {
        foughtBosses[i] = 0;
    }
}

void levelUp() {
    if (player.level >= player.max_level) {
        printfInInformationBox(3, "[!] 레벨업을 할 수 없습니다. 최대 레벨에 도달했습니다.");
        return;
    }

    printfInInformationBox(1, "[!] '플레이어'가 레벨업을 했습니다! 현재 레벨: %d", player.level);
    skipAbleSleep(1500);
    printfInInformationBox(1, "[+] 체력이 %d만큼 증가했습니다. [체력: %d -> %d]", player.hp_plus_per_level, player.max_hp, player.max_hp + player.hp_plus_per_level);
    skipAbleSleep(1500);
    printfInInformationBox(1, "[+] 마나가 %d만큼 증가했습니다. [마나: %d -> %d]", player.mp_plus_per_level, player.max_mp, player.max_mp + player.mp_plus_per_level);
    skipAbleSleep(1500);
    printfInInformationBox(1, "[+] 공격력이 %d만큼 증가했습니다. [공격력: %d -> %d]", player.atk_plus_per_level, player.atk, player.atk + player.atk_plus_per_level);
    skipAbleSleep(1500);
    printfInInformationBox(1, "[+] 방어력이 %d만큼 증가했습니다. [방어력: %d -> %d]", player.def_plus_per_level, player.def, player.def + player.def_plus_per_level);
    skipAbleSleep(1500);
    printfInInformationBox(1, "[+] 크리티컬 확률이 %d만큼 증가했습니다. [크리티컬 확률: %d -> %d]", player.critical_plus_per_level, player.critical_chance, player.critical_chance + player.critical_plus_per_level);

    player.max_hp += player.hp_plus_per_level;
    player.max_mp += player.mp_plus_per_level;

    player.atk += player.atk_plus_per_level;
    player.def += player.def_plus_per_level;
    player.critical_chance += player.critical_plus_per_level;
    player.level++;

}

void fightEnemy(struct enemy_stats enemy) {
    skipAbleSleep(1000);
    setPlayerMove(0);
    player_fight = 1;
    setReservedScreenBuffer(getCurrentScreenBufferIndex(), TRUE);
    clearScreenBufferByIndex(getNextScreenBufferIndex());
    switchNextScreenBuffer();

    skipAbleSleep(1000);
    struct Point center = getPrintCenter(enemy.ascii_art[0]);
    setColor(getCurrentScreenBuffer(), BLACK, enemy.color);
    for (int i = 0; i < asciiArtLength(enemy); i++) {
        printfXY(getCurrentScreenBuffer(), center.x, i + 7, enemy.ascii_art[i]);
    }
    resetColor(getCurrentScreenBuffer());

    int player_hp = player.max_hp;
    int player_mp = player.max_mp;

    int enemy_hp = enemy.hp;
    int enemy_mp = enemy.mp;

    int turn = rand() % 2; // 0: player turn, 1: enemy turn

    while (1) {
        printEdgeLines(getCurrentScreenBuffer(), zz, (struct Point) { getConsoleSize().x - 1, getAvailableConsoleHeight() + 1 });

        for (int i = 1; i < getConsoleSize().x - 1; i++) {
            printfXY(getCurrentScreenBuffer(), i, getAvailableConsoleHeight() - 7, "─");
        }

        for (int i = 33; i < 40; i++) {
            printfXY(getCurrentScreenBuffer(), 50, i, "│");
        }

        int damage = 0;
        if (turn == 0) {
            printfInInformationBox(0, "[!] '플레이어'의 턴입니다.");
            if (printSelectAction(HORIZONTAL,
                                  (struct Point) { 0, 33 },
                                  (struct Point) { 50, 40 },
                                  9,
                                  (char *[]) {"공격", "스킬"},
                                  2)) {
                switch (selectedIndex) {
                    case 0:
                        printfInInformationBox(0, "[!] '플레이어'가 '%s'을(를) 공격합니다.", enemy.enemy_name);
                        skipAbleSleep(1000);
                        damage = player.atk - enemy.def;

                        if (criticalCheck()) {
                            damage *= player.critical_damage_multiplier;
                            printfInInformationBox(3, "[!] '크리티컬 히트!'");
                            skipAbleSleep(1000);
                        }
                        break;
                    case 1:
                        printfInInformationBox(1, "[!] '플레이어'가 '스킬'을 사용했습니다.");
                        skipAbleSleep(1000);
                        // TODO: Add skill
                        break;
                    default:
                        break;
                }

                // GUI effect
                setColor(getCurrentScreenBuffer(), BLACK, RED);
                for (int i = 0; i < asciiArtLength(enemy); i++) {
                    printfXY(getCurrentScreenBuffer(), center.x, i + 7, enemy.ascii_art[i]);
                }
                resetColor(getCurrentScreenBuffer());
                skipAbleSleep(250);
                setColor(getCurrentScreenBuffer(), BLACK, enemy.color);
                for (int i = 0; i < asciiArtLength(enemy); i++) {
                    printfXY(getCurrentScreenBuffer(), center.x, i + 7, enemy.ascii_art[i]);
                }
                resetColor(getCurrentScreenBuffer());
                // GUI effect

                if (damage < 0) damage = 0;
                enemy_hp -= damage;
                printfInInformationBox(2, "[!] '%s'에게 %d의 피해를 입혔습니다. ['%s'의 남은 체력 : %d]", enemy.enemy_name, damage, enemy.enemy_name, enemy_hp);
                skipAbleSleep(2000);
                turn = (turn + 1) % 2;
            }
        } else {
            printfInInformationBox(0, "[!] '%s'의 턴입니다.", enemy.enemy_name);
            skipAbleSleep(2000);
            if (enemy.skill_count != 0) {
                if (rand() % 5 == 0) {
                    renewSkill:

                    int randomSkills = rand() % enemy.skill_count;
                    if (enemy.skill[randomSkills].skill_mp_cost > enemy_mp) {
                        goto renewSkill;
                    } else {
                        enemy_mp -= enemy.skill[randomSkills].skill_mp_cost;
                        damage = enemy.skill[randomSkills].skill_damage;
                        printfInInformationBox(1, "[!] '%s'이(가) '%s'을(를) 사용했습니다.", enemy.enemy_name,
                                               enemy.skill[randomSkills].skill_name);
                        skipAbleSleep(1000);
                        goto attackEnemy;
                    }
                }
            }

            printfInInformationBox(1, "[!] '%s'이(가) '플레이어'를 공격합니다.", enemy.enemy_name);
            skipAbleSleep(1000);
            damage = enemy.atk - player.def;
            if (damage < 0) damage = 0;

            if (criticalCheckEnemy(enemy)) {
                damage *= enemy.critical_damage_multiplier;
                printfInInformationBox(3, "[!] '크리티컬 히트!'");
                skipAbleSleep(1000);
            }

            attackEnemy:
            // GUI effect
            setColor(getCurrentScreenBuffer(), BLACK, RED);
            printEdgeLines(getCurrentScreenBuffer(), zz, (struct Point) { getConsoleSize().x - 1, getAvailableConsoleHeight() + 1 });

            for (int i = 1; i < getConsoleSize().x - 1; i++) {
                printfXY(getCurrentScreenBuffer(), i, getAvailableConsoleHeight() - 7, "─");
            }

            for (int i = 33; i < 40; i++) {
                printfXY(getCurrentScreenBuffer(), 50, i, "│");
            }
            resetColor(getCurrentScreenBuffer());
            skipAbleSleep(250);
            printEdgeLines(getCurrentScreenBuffer(), zz, (struct Point) { getConsoleSize().x - 1, getAvailableConsoleHeight() + 1 });

            for (int i = 1; i < getConsoleSize().x - 1; i++) {
                printfXY(getCurrentScreenBuffer(), i, getAvailableConsoleHeight() - 7, "─");
            }

            for (int i = 33; i < 40; i++) {
                printfXY(getCurrentScreenBuffer(), 50, i, "│");
            }
            // GUI effect

            player_hp -= damage;
            printfInInformationBox(2, "[!] '플레이어'에게 %d의 피해를 입혔습니다. ['플레이어'의 남은 체력: %d]", damage, player_hp);
            skipAbleSleep(2000);
            turn = (turn + 1) % 2;
        }

        if (enemy_hp <= 0) {
            printfInInformationBox(3, "[!] '%s'을 물리쳤습니다.", enemy.enemy_name);
            skipAbleSleep(1000);
            levelUp();
            skipAbleSleep(1000);
            player_fight = 0;
            switchNextScreenBuffer();
            refreshInformationBox();

            printfXY(getCurrentScreenBuffer(), temp_pos.x, temp_pos.y, "  ");
            foughtSlimes[getIndexFromSlimePos((struct Point) { temp_pos.x, temp_pos.y })] = 1;
            setPlayerMove(1);
            break;
        }

        if (player_hp <= 0) {
            char *reason = (char *) malloc(sizeof(char) * 100);
            sprintf(reason, "'%s'에게 패배했습니다.", enemy.enemy_name);

            playerDead(reason);
            break;
        }
    }
}

extern void ExitGame();
void playerDead(char* reason) {
    player_dead = 1;
    printfInInformationBox(3, "[!] '플레이어'가 사망했습니다. 사망 원인: %s", reason);

    skipAbleSleep(1000);
    clearScreenBufferByIndex(getCurrentScreenBufferIndex());
    refreshScreenBuffer();

    printContinueAction(31);
    printfInInformationBox(1, "게임을 종료합니다.");
    skipAbleSleep(1000);

    free(foughtSlimes);
    free(foughtBosses);
    ExitGame();
}

void gameClear() {
    printfInInformationBox(3, "[!] 게임을 클리어했습니다. 축하합니다!");
    skipAbleSleep(1000);
    clearScreenBufferByIndex(getNextScreenBufferIndex());
    printfCenter(getNextScreenBuffer(), 13, "대충 스토리");
    switchNextScreenBuffer();
    skipAbleSleep(1000);

    printContinueAction(31);
    printfInInformationBox(1, "게임을 종료합니다.");
    skipAbleSleep(1000);

    free(foughtSlimes);
    free(foughtBosses);
    ExitGame();
}

int movePlayer(int direction) {
    temp_pos = getPlayerPosition();

    copyScreenBufferByIndex(getCurrentScreenBufferIndex(), getNextScreenBufferIndex());

    int playerCharLength = (int) utf8Strlen(player.character_symbol);

    switch (direction) {
        case 0:
            temp_pos.y--;
            break;
        case 1:
            temp_pos.x += playerCharLength;
            break;
        case 2:
            temp_pos.y++;
            break;
        case 3:
            temp_pos.x -= playerCharLength;
            break;
        default:
            return 0;
    }

    int crashed = isCrashed(player_map, (struct Point) {temp_pos.x, temp_pos.y });
//    printfInInformationBox(0, "[Debug] isCrashed: %d", crashed);
    switch (crashed) {
        case 5: // max relative position
//            printfInInformationBox(0, "[!] 이동할 수 없는 위치입니다.");
            return 0;
        case 1: // wall
//            printfInInformationBox(0, "[!] 벽에 부딪혔습니다.");
            return 0;
        case 2: // slime
            printfInInformationBox(0, "[!] 슬라임을 만났습니다.");
            fightEnemy(getSlimeStats());
            return 0;
        case 3: // boss
            for (int i = 0; i < slimeCount; i++) {
                if (foughtSlimes[i] == 0) {
                    printfInInformationBox(3, "[!] 슬라임을 모두 물리치지 않았습니다.");
                    return 0;
                }
            }

            printfInInformationBox(0, "[!] 마왕을 만났습니다.");
            fightEnemy(getBossStats());
            gameClear();
            return 0;
        case 4: // move another map
            printfInInformationBox(0, "[!] 다음 맵으로 이동합니다.");
            return 0;
        default:
            break;
    }

    // delete before position
    printfXY(getNextScreenBuffer(), player_pos.x, player_pos.y, "  ");
    // move player
    setColor(getNextScreenBuffer(), BLACK, SkyBlue);
    printfXY(getNextScreenBuffer(), temp_pos.x, temp_pos.y, player.character_symbol);
    resetColor(getNextScreenBuffer());
    switchNextScreenBuffer();

    player_pos = temp_pos;

    return 1;
}

int mapMovePlayer(int mapNum, struct Point pos) {
    free(foughtSlimes);
    free(foughtBosses);
}


struct Point getPlayerPosition() {
    return player_pos;
}

int getPlayerMap() {
    return player_map;
}

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
//    printfInInformationBox(0, "[Debug] random: %d, player.critical_chance: %d", random, player.critical_chance);

    return random <= player.critical_chance;
}