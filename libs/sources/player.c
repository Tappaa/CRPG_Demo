#include "../manager.h"

struct Point player_pos = { 0, 0 };
struct Point temp_pos = { 0, 0 };
int player_map = 0;
struct player_stats player = { };

int player_dead = 0;
int player_fight = 0;
int player_move = 0;

int player_hp = 0;
int player_mp = 0;

int enemy_hp = 0;
int enemy_mp = 0;

int *foughtSlimes = NULL;
int *foughtBosses = NULL;

void playerStatusUpdate(int hp, int mp) {
    struct Point s = {
            .x = 50,
            .y = 30
    };

    struct Point e = {
            .x = 158,
            .y = 40
    };

    clearConsoleArea(getCurrentScreenBuffer(), (struct Point) { s.x + 1, s.y + 1 }, (struct Point) { e.x - 1, e.y - 1 });

    struct Point HP = getPrintCenterByPos((struct Point) { s.x, 32 }, (struct Point) { e.x, 32 }, "체력 : ■■■■■■■■■■ (%02d/%02d)", hp, player.max_hp);
    struct Point MP = getPrintCenterByPos((struct Point) { s.x, 33 }, (struct Point) { e.x, 33 }, "마나 : ■■■■■■■■■■ (%02d/%02d)", mp, player.max_mp);

    int hp_bar = (int) ((double) hp / player.max_hp * 10);
    int mp_bar = (int) ((double) mp / player.max_mp * 10);

    HP = printfXY(getCurrentScreenBuffer(), HP.x, HP.y, "체력 : ");
    MP = printfXY(getCurrentScreenBuffer(), MP.x, MP.y, "마나 : ");
    printfAreaCenter(getCurrentScreenBuffer(), (struct Point) { s.x, 35 }, (struct Point) { e.x, 35 }, "공격력 : %d", player.atk);
    printfAreaCenter(getCurrentScreenBuffer(), (struct Point) { s.x, 36 }, (struct Point) { e.x, 36 }, "방어력 : %d", player.def);
    printfAreaCenter(getCurrentScreenBuffer(), (struct Point) { s.x, 37 }, (struct Point) { e.x, 37 }, "크리티컬 확률 : %d%%", player.critical_chance);
    printfAreaCenter(getCurrentScreenBuffer(), (struct Point) { s.x, 38 }, (struct Point) { e.x, 38 }, "레벨 : %d LV", player.level);

    setColor(getCurrentScreenBuffer(), BLACK, RED);
    for (int i = 0; i < hp_bar; i++) {
        HP = printfXY(getCurrentScreenBuffer(), HP.x, HP.y, "■");
    }
    setColor(getCurrentScreenBuffer(), BLACK, GRAY);
    for (int i = 0; i < 10 - hp_bar; i++) {
        HP = printfXY(getCurrentScreenBuffer(), HP.x, HP.y, "■");
    }

    printfXY(getCurrentScreenBuffer(), HP.x, HP.y, " (%02d/%02d)", hp, player.max_hp);

    setColor(getCurrentScreenBuffer(), BLACK, BLUE);
    for (int i = 0; i < mp_bar; i++) {
        MP = printfXY(getCurrentScreenBuffer(), MP.x, MP.y, "■");
    }
    setColor(getCurrentScreenBuffer(), BLACK, GRAY);
    for (int i = 0; i < 10 - mp_bar; i++) {
        MP = printfXY(getCurrentScreenBuffer(), MP.x, MP.y, "■");
    }
    resetColor(getCurrentScreenBuffer());

    printfXY(getCurrentScreenBuffer(), MP.x, MP.y, " (%02d/%02d)", mp, player.max_mp);
}

void skillStatusUpdate(int index) {
    struct Point s = {
            .x = 50,
            .y = 30
    };

    struct Point e = {
            .x = 90,
            .y = 40
    };

    clearConsoleArea(getCurrentScreenBuffer(), (struct Point) { s.x + 1, s.y + 1 }, (struct Point) { e.x - 1, e.y - 1 });
    printEdgeLines(getCurrentScreenBuffer(), (struct Point) { s.x + 1, s.y + 1 }, (struct Point) { e.x - 1, e.y - 1 });

    setColor(getCurrentScreenBuffer(), GRAY, BLACK);
    printfAreaCenter(getCurrentScreenBuffer(), (struct Point) { s.x, 32 }, (struct Point) { e.x, 33 }, "스킬 : %s", player.skill[index].skill_name);
    setColor(getCurrentScreenBuffer(), GRAY, RED);
    printfAreaCenter(getCurrentScreenBuffer(), (struct Point) { s.x, 34 }, (struct Point) { e.x, 34 }, "공격력 : %d", player.skill[index].skill_damage);
    setColor(getCurrentScreenBuffer(), GRAY, BLUE);
    printfAreaCenter(getCurrentScreenBuffer(), (struct Point) { s.x, 35 }, (struct Point) { e.x, 35 }, "마나 소모 : %d", player.skill[index].skill_mp_cost);
    setColor(getCurrentScreenBuffer(), GRAY, GREEN);
    printfAreaCenter(getCurrentScreenBuffer(), (struct Point) { s.x, 36 }, (struct Point) { e.x, 36 }, "회복량 : %d", player.skill[index].skill_heal);
    setColor(getCurrentScreenBuffer(), GRAY, darkGreen);
    printfAreaCenter(getCurrentScreenBuffer(), (struct Point) { s.x, 37 }, (struct Point) { e.x, 37 }, "사용 가능 레벨 : %d", player.skill[index].minimum_level);
    setColor(getCurrentScreenBuffer(), GRAY, BLACK);
    printfAreaCenter(getCurrentScreenBuffer(), (struct Point) { s.x, 38 }, (struct Point) { e.x, 38 }, "사용 가능 여부 : %s", (player.skill[index].minimum_level <= player.level) && (player.skill[index].skill_mp_cost <= player_mp) ? "가능" : "불가능");
}

void initPlayer() {
    strcpy(player.character_symbol, "웃");
    player.max_hp = 35;
    player.hp_plus_per_level = 15; // max hp = 30 + 15 * 6 = 130

    player.max_mp = 45;
    player.mp_plus_per_level = 5; // max mp = 45 + 5 * 6 = 75

    player.atk = 8;
    player.atk_plus_per_level = 2; // max atk = 8 + 2 * 6 = 20

    player.def = 0;
    player.def_plus_per_level = 2; // max def = 0 + 2 * 6 = 12

    player.level = 1;
    player.max_level = 6;

    player.critical_chance = 5;
    player.critical_plus_per_level = 1; // max critical chance = 5 + 1 * 5 = 10
    player.critical_damage_multiplier = 2;

    struct skills skill1 = {
            .skill_name = "성스러운 칼",
            .skill_damage = 30,
            .skill_mp_cost = 40,
            .skill_heal = 0,
            .minimum_level = player.max_level
    };

    struct skills skill2 = {
            .skill_name = "단결된 의지",
            .skill_damage = 0,
            .skill_mp_cost = 10,
            .skill_heal = 40,
            .minimum_level = 3
    };

    player.skill_count = 2;
    player.skill[0] = skill1;
    player.skill[1] = skill2;

    player.miss_chance = 4;
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

int fightEnemy(struct enemy_stats enemy) {
    skipAbleSleep(1000);
    setPlayerMove(0);
    player_fight = 1;
    setReservedScreenBuffer(getCurrentScreenBufferIndex(), TRUE);
    clearScreenBufferByIndex(getNextScreenBufferIndex());
    switchNextScreenBuffer();

    skipAbleSleep(1000);
    struct Point center = getPrintCenter(enemy.ascii_art[0]);
    setColor(getCurrentScreenBuffer(), BLACK, enemy.color);
    for (int i = 0; i < 20; i++) { // 20: ascii art length
        printfXY(getCurrentScreenBuffer(), center.x, i + 5, enemy.ascii_art[i]);
    }
    resetColor(getCurrentScreenBuffer());

    // enemy status update
    enemy.hp += enemy.hp_per_level * (player.level - 1);
    enemy.mp += enemy.mp_per_level * (player.level - 1);
    enemy.atk += enemy.atk_per_level * (player.level - 1);
    enemy.def += enemy.def_per_level * (player.level - 1);
    // enemy status update

    player_hp = player.max_hp;
    player_mp = player.max_mp;

    enemy_hp = enemy.hp;
    enemy_mp = enemy.mp;

    int turn = rand() % 2; // 0: player turn, 1: enemy turn

    playerStatusUpdate(player_hp, player_mp);

    while (1) {
        printEdgeLines(getCurrentScreenBuffer(), zz, (struct Point) { getConsoleSize().x - 1, getAvailableConsoleHeight() + 1 });

        for (int i = 1; i < getConsoleSize().x - 1; i++) {
            printfXY(getCurrentScreenBuffer(), i, getAvailableConsoleHeight() - 9, "─");
        }

        for (int i = 31; i < 40; i++) {
            printfXY(getCurrentScreenBuffer(), 50, i, "│");
        }

        int damage = 0;
        if (turn == 0) {
            printfInInformationBox(0, "[!] '플레이어'의 턴입니다.");

            returnAttackSelectP:
            if (printSelectAction(HORIZONTAL,
                                  (struct Point) { 0, 31 },
                                  (struct Point) { 50, 40 },
                                  9,
                                  (char *[]) {"공격", "스킬", NULL},
                                  2, FALSE, NULL)) {
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
                        if (printSelectAction(HORIZONTAL,
                                              (struct Point) { 0, 31 },
                                              (struct Point) { 50, 40 },
                                              7,
                                              (char *[]) { player.skill[0].skill_name, player.skill[1].skill_name, NULL },
                                              2, TRUE, skillStatusUpdate)) {
                            playerStatusUpdate(player_hp, player_mp);
                            switch (selectedIndex) {
                                case 0:
                                case 1:
                                    if (player.skill[selectedIndex].minimum_level > player.level) {
                                        printfInInformationBox(3, "[!] 레벨이 부족합니다.");
                                        skipAbleSleep(1000);
                                        playerStatusUpdate(player_hp, player_mp);
                                        goto returnAttackSelectP;
                                    }

                                    if (player.skill[selectedIndex].skill_mp_cost > player_mp) {
                                        printfInInformationBox(3, "[!] 마나가 부족합니다.");
                                        skipAbleSleep(1000);
                                        playerStatusUpdate(player_hp, player_mp);
                                        goto returnAttackSelectP;
                                    }

                                    player_mp -= player.skill[selectedIndex].skill_mp_cost;
                                    damage = player.skill[selectedIndex].skill_damage;
                                    printfInInformationBox(0, "[!] '플레이어'가 '%s'을(를) 사용했습니다. ['플레이어'의 남은 마나: %d]", player.skill[selectedIndex].skill_name, player_mp);
                                    skipAbleSleep(1000);

                                    if (player.skill[selectedIndex].skill_heal != 0) {
                                        player_hp += player.skill[selectedIndex].skill_heal;
                                        if (player_hp > player.max_hp) player_hp = player.max_hp;

                                        printfInInformationBox(1, "[!] '%s'이(가) %d만큼 회복했습니다. ['플레이어'의 남은 체력: %d]", player.skill[selectedIndex].skill_name, player.skill[selectedIndex].skill_heal, player_hp);
                                        skipAbleSleep(1000);
                                    }
                                    break;
                                default:
//                                    printfInInformationBox(3, "[!] 잘못된 선택입니다. 다시 선택해주세요.");
                                    playerStatusUpdate(player_hp, player_mp);
                                    goto returnAttackSelectP;
                            }
                        }
                        break;
                    default:
                        break;
                }


                if (!missCheckEnemy(enemy)) {
                    if (damage < 0) damage = 0;
                    if (damage > 0) {
                        // GUI effect
                        setColor(getCurrentScreenBuffer(), BLACK, RED);
                        for (int i = 0; i < 20; i++) {
                            printfXY(getCurrentScreenBuffer(), center.x, i + 5, enemy.ascii_art[i]);
                        }
                        resetColor(getCurrentScreenBuffer());
                        skipAbleSleep(250);
                        setColor(getCurrentScreenBuffer(), BLACK, enemy.color);
                        for (int i = 0; i < 20; i++) {
                            printfXY(getCurrentScreenBuffer(), center.x, i + 5, enemy.ascii_art[i]);
                        }
                        resetColor(getCurrentScreenBuffer());
                        // GUI effect

                        enemy_hp -= damage;
                        printfInInformationBox(2, "[*] '%s'에게 %d의 피해를 입혔습니다. ['%s'의 남은 체력 : %d]", enemy.enemy_name, damage, enemy.enemy_name, enemy_hp);
                    }
                } else {
                    printfInInformationBox(3, "[X] '플레이어'의 공격이 빗나갔습니다.");
                }
                skipAbleSleep(2000);
                turn = (turn + 1) % 2;
            }
        } else {
            printfInInformationBox(0, "[!] '%s'의 턴입니다.", enemy.enemy_name);
            skipAbleSleep(2000);
            if (enemy.skill_count != 0) {
                if (rand() % 10 == 0) {
                    renewSkill:

                    int randomSkills = rand() % enemy.skill_count;
                    if (enemy.skill[randomSkills].skill_mp_cost > enemy_mp) {
                        goto renewSkill;
                    } else {
                        enemy_mp -= enemy.skill[randomSkills].skill_mp_cost;
                        damage = enemy.skill[randomSkills].skill_damage;
                        printfInInformationBox(1, "[!] '%s'이(가) '%s'을(를) 사용했습니다. ['%s'의 남은 마나: %d]", enemy.enemy_name, enemy.skill[randomSkills].skill_name, enemy.enemy_name, enemy_mp);
                        skipAbleSleep(1000);

                        if (enemy.skill[randomSkills].skill_heal != 0) {
                            enemy_hp += enemy.skill[randomSkills].skill_heal;
                            if (enemy_hp > enemy.hp) enemy_hp = enemy.hp;

                            printfInInformationBox(2, "[!] '%s'이(가) %d만큼 회복했습니다. ['%s'의 남은 체력: %d]", enemy.enemy_name, enemy.skill[randomSkills].skill_heal, enemy.enemy_name, enemy_hp);
                            skipAbleSleep(1000);
                        }
                        goto attackEnemy;
                    }
                }
            }

            printfInInformationBox(1, "[!] '%s'이(가) '플레이어'를 공격합니다.", enemy.enemy_name);
            skipAbleSleep(1000);
            damage = enemy.atk - player.def;

            if (criticalCheckEnemy(enemy)) {
                damage *= enemy.critical_damage_multiplier;
                printfInInformationBox(3, "[!] '크리티컬 히트!'");
                skipAbleSleep(1000);
            }

            attackEnemy:
            if (!missCheck()) {
                if (damage < 0) damage = 0;
                if (damage > 0) {
                    // GUI effect
                    setColor(getCurrentScreenBuffer(), BLACK, RED);
                    printEdgeLines(getCurrentScreenBuffer(), zz, (struct Point) { getConsoleSize().x - 1, getAvailableConsoleHeight() + 1 });

                    for (int i = 1; i < getConsoleSize().x - 1; i++) {
                        printfXY(getCurrentScreenBuffer(), i, getAvailableConsoleHeight() - 9, "─");
                    }

                    for (int i = 31; i < 40; i++) {
                        printfXY(getCurrentScreenBuffer(), 50, i, "│");
                    }
                    resetColor(getCurrentScreenBuffer());
                    skipAbleSleep(250);
                    printEdgeLines(getCurrentScreenBuffer(), zz, (struct Point) { getConsoleSize().x - 1, getAvailableConsoleHeight() + 1 });

                    for (int i = 1; i < getConsoleSize().x - 1; i++) {
                        printfXY(getCurrentScreenBuffer(), i, getAvailableConsoleHeight() - 9, "─");
                    }

                    for (int i = 31; i < 40; i++) {
                        printfXY(getCurrentScreenBuffer(), 50, i, "│");
                    }
                    // GUI effect

                    player_hp -= damage;
                    printfInInformationBox(2, "[*] '플레이어'에게 %d의 피해를 입혔습니다. ['플레이어'의 남은 체력: %d]", damage, player_hp);
                }
            } else {
                printfInInformationBox(3, "[X] '%s'의 공격이 빗나갔습니다.", enemy.enemy_name);
            }
            skipAbleSleep(2000);
            turn = (turn + 1) % 2;
        }

        if (enemy_hp <= 0) {
            printfInInformationBox(3, "[!] '%s'을 물리쳤습니다.", enemy.enemy_name);
            skipAbleSleep(1000);
            levelUp();
            skipAbleSleep(1000);
            switchNextScreenBuffer();
            refreshInformationBox();

            printfXY(getCurrentScreenBuffer(), temp_pos.x, temp_pos.y, "  ");
            foughtSlimes[getIndexFromSlimePos((struct Point) { temp_pos.x, temp_pos.y })] = 1;
            setPlayerMove(1);
            return 1;
        }

        if (player_hp <= 0) {
            char *reason = (char *) malloc(sizeof(char) * 100);
            sprintf(reason, "'%s'에게 패배했습니다.", enemy.enemy_name);

            playerDead(reason);
            return 0;
        }

        playerStatusUpdate(player_hp, player_mp);
    }
}

extern void ExitGame();
void playerDead(char* reason) {
    player_dead = 1;
    printfInInformationBox(3, "[!] '플레이어'가 사망했습니다. 사망 원인: %s", reason);

    skipAbleSleep(1000);
    clearScreenBufferByIndex(getCurrentScreenBufferIndex());
    refreshScreenBuffer();
    refreshInformationBox();

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
    printfCenter(getNextScreenBuffer(), 13, "전투의 절정에서 렌은 자신의 무기가 빛을 발하는 것을 느꼈다.");
    printfCenter(getNextScreenBuffer(), 14, "그건 마을 사람들의 희망과 응원이 깃든 힘이었다.");
    printfCenter(getNextScreenBuffer(), 15, "렌은 마지막 일격으로 마왕의 심장을 정확히 겨냥해 무기를 내리꽂았다.");
    printfCenter(getNextScreenBuffer(), 16, "마왕은 땅으로 떨어지며 그의 영혼은 사라졌다.");
    printfCenter(getNextScreenBuffer(), 17, "마을 사람들은 렌을 환영하며 마을로 돌아가도록 했다.");
    printfCenter(getNextScreenBuffer(), 18, "렌은 마을로 돌아가며 마을 사람들과 함께 행복한 시간을 보냈다.");
    printfCenter(getNextScreenBuffer(), 19, "그리고 마을 사람들은 렌을 영웅으로 기억했다.");
    switchNextScreenBuffer();
    refreshInformationBox();
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
            if (fightEnemy(getBossStats())) gameClear();
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

int missCheck() {
    int random = (rand() % 100) + 1;

    return random <= player.miss_chance;
}