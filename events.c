#include "libs/system.h"

extern unsigned long long now_ticks;
// extern

int cheat_mode = 0;
void secret_code_runner(int* pass, int enter, int size) {
    // already enabled
    if (cheat_mode == 1) return;

    // lowercase
    if (enter >= 97 && enter <= 122) enter -= 32;
    // insert 0 index
    int temp = enter, temp2;
    for (int i = 0; i <= size; i++) {
        temp2 = pass[i];
        pass[i] = temp;
        temp = temp2;
//        printf("%d\t", pass[i]);
    }
    //printf("\n");
    // U D U D L R L R B A
    // A B R L R L D U D U
    if (pass[0] == KEY_A && pass[1] == KEY_B && pass[2] == KEY_RIGHT && pass[3] == KEY_LEFT && pass[4] == KEY_RIGHT && pass[5] == KEY_LEFT && pass[6] == KEY_DOWN && pass[7] == KEY_UP && pass[8] == KEY_DOWN && pass[9] == KEY_UP) {
        cheat_mode = 1;
        printfInInformationBox(0, "[!] 치트모드가 활성화 되었습니다. [%d]", cheat_mode);
    }
}

int animation_pakts = 0;
int local_ticks_pakts = 0; // 64tick = 1sec
void intro() {
    switch (now_ticks) {
        case 1000:
            printfXY(0, 0, "Welcome to C-RPG");
            printfXY(0, 1, "This is a simple RPG game made with C language.");
            break;
        case 4000:
            clearConsoleLines(0, getUseableConsoleHeight());
            printfCenter(8, "C-RPG");
            printfCenter(9, "2024.10.15");
            printfCenter(10, "Made by. R,M,C");
            break;
        case 5000:
        case 8000:
            printfCenter(30, "Loading.  ");
            break;
        case 6000:
        case 9000:
            printfCenter(30, "Loading.. ");
            break;
        case 7000:
        case 10000:
            printfCenter(30, "Loading...");
            break;
        case 11000:
            printfCenter(30, "Loading complete.");
            break;
        case 13000:
            printfCenter(31, "Press any key to start.");
            printfInInformationBox(1, "게임을 시작할 준비가 되었습니다.");
            break;
        default:
            if (now_ticks > 13000) {
                local_ticks_pakts++;
                if (animation_pakts == 0 && local_ticks_pakts == 32) {
                    printfCenter(31, "                       ");

                    animation_pakts = 1;
                } else if (animation_pakts == 1 && local_ticks_pakts == 64) {
                    printfCenter(31, "Press any key to start.");
                    animation_pakts = 0;
                    local_ticks_pakts = 0;
                }
            }
            break;
    }
}