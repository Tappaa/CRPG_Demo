#include <stdio.h>
#include "libs/system.h"

extern unsigned long long start_tick;
extern unsigned long long now_ticks;
extern struct Key keyData;

extern void ExitGame();
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

int intro__animation = 0;
int intro__local_ticks = 0; // 64tick = 1sec
int intro__dead = 0;
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
                intro__local_ticks++;
                if (intro__animation == 0 && intro__local_ticks == 32) {
                    clearConsoleLines(31, 31);
                    intro__animation = 1;
                } else if (intro__animation == 1 && intro__local_ticks == 64) {
                    printfCenter(31, "Press any key to start.");
                    intro__animation = 0;
                    intro__local_ticks = 0;
                }

                if (keyData.isPressed == 1) {
                    intro__dead = 1;
                    clearConsoleLines(20, getUseableConsoleHeight());

                    // draw buttons
                    printfCenter(30, "---------------");
                    printfXY(getPrintCenter("    |  게임 시작  |").x, 31, "    |  ");
                    setColor(WHITE, BLACK);
                    printf("게임 시작");
                    setColor(BLACK, WHITE);
                    printf("  |");
                    printfCenter(32, "    |  게임 종료  |");
                    printfCenter(33, "---------------");
                }
            }
            break;
    }
}

int start_screen_button__selected = 0;
int start_screen_button__selected__local_ticks = 0;
int start_screen_button__dead = 0;
void start_screen_button() {
    if (intro__dead == 1) {
        // button move delay
        if (start_screen_button__selected__local_ticks > 0) {
            start_screen_button__selected__local_ticks--;
        }
        // select button
        if (start_screen_button__selected < 1 && keyData.key == KEY_DOWN && keyData.isPressed == 1 && start_screen_button__selected__local_ticks == 0) {
            start_screen_button__selected++;
            start_screen_button__selected__local_ticks = 16;
            printfInInformationBox(0, "[Debug] Selected button: %d", start_screen_button__selected);
        } else if (start_screen_button__selected > 0 && keyData.key == KEY_UP && keyData.isPressed == 1 && start_screen_button__selected__local_ticks == 0) {
            start_screen_button__selected--;
            start_screen_button__selected__local_ticks = 16;
            printfInInformationBox(0, "[Debug] Selected button: %d", start_screen_button__selected);
        }

        // draw buttons
        if (start_screen_button__selected == 0) {
            printfXY(getPrintCenter("    |  게임 시작  |").x, 31, "    |  ");
            setColor(WHITE, BLACK);
            printf("게임 시작");
            resetColor();
            printf("  |");
            printfCenter(32, "    |  게임 종료  |");
        } else if (start_screen_button__selected == 1) {
            printfCenter(31, "    |  게임 시작  |");
            printfXY(getPrintCenter("    |  게임 종료  |").x, 32, "    |  ");
            setColor(WHITE, BLACK);
            printf("게임 종료");
            resetColor();
            printf("  |");
        }

        // button click
        if (keyData.key == KEY_ENTER && keyData.isPressed == 1) {
            if (start_screen_button__selected == 0) {
                printfInInformationBox(1, "게임을 시작합니다.");

                start_screen_button__dead = 1;
                clearConsoleLines(0, getUseableConsoleHeight());
            } else if (start_screen_button__selected == 1) {
                printfInInformationBox(1, "게임을 종료합니다.");
                ExitGame();
            }
        }
    }
}

int story__animation = 0;
int story__local_ticks = 0;
int story__dead = 0;
void story() {
    if (start_screen_button__dead == 1) {
        story__local_ticks++;

        switch (story__local_ticks) {
            case 64:
                printfCenter(13, "대충 스토리 설명");
                break;
            case 320:
                clearConsoleLines(13, 13);
                printfCenter(13, "조작키");
                printfCenter(14, "↑, ↓, ←, → : 이동");
                printfCenter(15, "Enter : 확인");
                printfCenter(16, "ESC : 뒤로가기, 메뉴");
                printfCenter(17, "\\ : ????");
                break;
            default:
                if (story__local_ticks > 320) {
                    if (story__animation == 0 && story__local_ticks == 352) {
                        clearConsoleLines(31, 31);
                        story__animation = 1;
                    } else if (story__animation == 1 && story__local_ticks == 384) {
                        printfCenter(31, "Press any key to start.");
                        story__animation = 0;
                        story__local_ticks = 320;
                    }

                    if (keyData.isPressed == 1) {
                        story__dead = 1;
                        clearConsoleLines(0, getUseableConsoleHeight());

                    }
                }
                break;
        }
    }
}