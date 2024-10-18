#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include "libs/system.h"

struct Point con_size;

extern int cheat_mode;
unsigned long long start_tick;
unsigned long long now_ticks;

extern void secret_code_runner(int* pass, int enter, int size);
extern void intro();
int main() {
    // get now time with milliseconds
    start_tick = GetTickCount64();
    system("chcp 65001");
    system("mode con cols=160 lines=43 | title \"C-RPG | Alpha Test\"");
    for (int i = 0; i <= getConsoleSize().x; i++) {
        gotoXY(i, getUseableConsoleHeight() + 1);
        printf("-");
    }
    con_size = getConsoleSize();

    // do not change console size
    printfInInformationBox(3, "[경고] 콘솔창 크기를 변경하지 마세요. (변경시 프로그램이 종료됩니다)");

    int key;
    int secret[10] = { 0 };
    unsigned long long before_tick = 0;
    while (1) { // runtime loop
        now_ticks = GetTickCount64() - start_tick; // get now_ticks_from_time from start

        // check console size change
        if (con_size.x != getConsoleSize().x || con_size.y != getConsoleSize().y) {
            exit(0);
        }

        if (_kbhit()) {
            key = _getch();
            if (key == 224) { // detect movement key
                key = _getch();
                secret_code_runner(secret, key, sizeof(secret) / sizeof(int));
                switch (key) {
                    case KEY_LEFT:
//                        printf("%d, 왼쪽으로 이동\n", key);
                        break;
                    case KEY_RIGHT:
//                        printf("%d, 오른쪽으로 이동\n", key);
                        break;
                    case KEY_UP:
//                        printf("%d, 위로 이동\n", key);
                        break;
                    case KEY_DOWN:
//                        printf("%d, 아래로 이동\n", key);
                        break;
                    default:
                        break;
                }
            }
            else { // detect other key
                secret_code_runner(secret, key, sizeof(secret) / sizeof(int));
//                printf("%d, %c 입력\n", key, key);
            }
        }

        // duplicate check
        if (now_ticks == before_tick) continue;
        // write after this line
        intro();
        before_tick = now_ticks;
    }
}

