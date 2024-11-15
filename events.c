#include "libs/manager.h"

extern unsigned long long start_tick;
extern unsigned long long now_ticks;

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
//        printf_Buffer(getCurrentScreenBuffer(), "%d\t", pass[i]);
    }
    //printf_Buffer(getCurrentScreenBuffer(), "\n");
    // U D U D L R L R B A
    // A B R L R L D U D U
    if (pass[0] == KEY_A && pass[1] == KEY_B && pass[2] == KEY_RIGHT && pass[3] == KEY_LEFT && pass[4] == KEY_RIGHT && pass[5] == KEY_LEFT && pass[6] == KEY_DOWN && pass[7] == KEY_UP && pass[8] == KEY_DOWN && pass[9] == KEY_UP) {
        cheat_mode = 1;
        printfInInformationBox(0, "[!] 치트모드가 활성화 되었습니다. [%d]", cheat_mode);
    }
}

int intro__dead = 0;
void intro() {
    intro__dead = 1;

    initPlayer();
    createPlayer(0, getPlayerStartLocation(0));

    switchNextScreenBuffer();

    return; //  : skip intro
    switch (now_ticks) {
        case 1000:
            printfXY(getCurrentScreenBuffer(), 0, 0, "Welcome to C-RPG");
            printfXY(getCurrentScreenBuffer(), 0, 1, "This is a simple RPG game made with C language.");

            int length = asciiArtLength(getSlimeStats());
            for (int i = 0; i < length; i++) {
                printfXY(getCurrentScreenBuffer(), 50, 10 + i, getSlimeStats().ascii_art[i]);
            }
            break;
        case 4000:
            clearConsoleLines(getCurrentScreenBuffer(), 0, getAvailableConsoleHeight());
            printfCenter(getCurrentScreenBuffer(), 8, "C-RPG");
            printfCenter(getCurrentScreenBuffer(), 9, "2024.10.15");
            printfCenter(getCurrentScreenBuffer(), 10, "Made by. R,M,C");
            printfCenter(getCurrentScreenBuffer(), 11, "Version. %s", game_version);
            copyScreenBufferByIndex(getCurrentScreenBufferIndex(), getNextScreenBufferIndex());
            break;
        case 5000:
        case 8000:
            printfCenter(getNextScreenBuffer(), 30, "Loading.  ");
            switchNextScreenBuffer();
            break;
        case 6000:
        case 9000:
            printfCenter(getNextScreenBuffer(), 30, "Loading.. ");
            switchNextScreenBuffer();
            break;
        case 7000:
        case 10000:
            printfCenter(getNextScreenBuffer(), 30, "Loading...");
            switchNextScreenBuffer();
            break;
        case 11000:
            printfCenter(getNextScreenBuffer(), 30, "Loading complete.");
            copyScreenBufferByIndex(getNextScreenBufferIndex(), getCurrentScreenBufferIndex());
            switchNextScreenBuffer();
            break;
        case 13000:
            printfCenter(getNextScreenBuffer(), 31, "계속하려면 아무 키나 누르세요.");
            printfInInformationBox(1, "게임을 시작할 준비가 되었습니다.");
            switchNextScreenBuffer();
            break;
        default:
            if (now_ticks > 13000) {
                printContinueAction(31);

                intro__dead = 1;
                clearConsoleLines(getNextScreenBuffer(), 20, getAvailableConsoleHeight());

                // draw buttons
                struct Point start = { 72, 30 };
                struct Point end = { 85, 33 };
                switchNextScreenBuffer();
                printEdgeLines(getCurrentScreenBuffer(), start, end);
                if (printSelectAction(VERTICAL,
                                      start,
                                      end,
                                      0,
                                      (char *[]) { "게임 시작", "게임 종료" },
                                      2)) {
                    switch (selectedIndex) {
                        case 0:
                            printfInInformationBox(1, "게임을 시작합니다.");

                            clearScreenBufferByIndex(getNextScreenBufferIndex());
                            switchNextScreenBuffer();
                            break;
                        case 1:
                            printfInInformationBox(1, "게임을 종료합니다.");
                            ExitGame();
                            break;
                        default:
                            break;
                    }
                }
            }
            break;
    }
}

int story__local_ticks = 0;
int story__page = 0;
int story__dead = 0;
void story() {
    if (intro__dead == 1) {
        story__dead = 1;
        return; //  : skip intro

        story__local_ticks++;

        switch (story__page) {
            case 0:
                if (story__local_ticks == 64) {
                    clearScreenBufferByIndex(getNextScreenBufferIndex());
                    printfCenter(getNextScreenBuffer(), 13, "옛날 옛적, 한 작은 마을에 \"슬라임 퇴치 전문가\"라는 직업을 가진 평범한 청년, 렌이 살고 있었다.");
                    printfCenter(getNextScreenBuffer(), 14, "렌은 보통 마을 주변을 돌아다니며 물컹물컹한 슬라임들을 잡는 일을 했다.");
                    printfCenter(getNextScreenBuffer(), 15, "누구에게나 무난히 깔끔하게 치우는 슬라임 청소부 역할이었지만, 늘 뭔가 더 큰 일을 해보고 싶다는 마음이 있었다.");

                    printfCenter(getNextScreenBuffer(), 17, "어느 날, 마을에 소문이 돌았다.");
                    printfCenter(getNextScreenBuffer(), 18, "\"마왕이 되살아나 세상을 어둠에 물들이려 한다!\" 마왕이 있는 곳은 험준한 산의 정상, 고대의 성채라 했다.");
                    printfCenter(getNextScreenBuffer(), 19, "렌은 용기를 내어, 단단히 준비한 자신의 무기와 함께 길을 나섰다.");
                    switchNextScreenBuffer();
                } else if (story__local_ticks > 64) {
                    printContinueAction(31);

                    story__page++;
                    story__local_ticks = 0;
                    clearScreenBufferByIndex(getNextScreenBufferIndex());
                    switchNextScreenBuffer();
                }
                break;
            case 1:
                if (story__local_ticks == 64) {
                    clearScreenBufferByIndex(getNextScreenBufferIndex());
                    printfCenter(getNextScreenBuffer(), 13, "조작키");
                    printfCenter(getNextScreenBuffer(), 14, "↑, ↓, ←, → : 이동");
                    printfCenter(getNextScreenBuffer(), 15, "Enter : 확인");
                    printfCenter(getNextScreenBuffer(), 16, "ESC : 뒤로가기, 메뉴");
                    printfCenter(getNextScreenBuffer(), 17, "\\ : ????");
                    switchNextScreenBuffer();
                } else if (story__local_ticks > 64) {
                    printContinueAction(31);

                    story__page++;
                    story__local_ticks = 0;
                    clearScreenBufferByIndex(getNextScreenBufferIndex());
                    switchNextScreenBuffer();
                    clearScreenBufferByIndex(getNextScreenBufferIndex());
                    Sleep(500);

                    initPlayer();
                    createPlayer(0, getPlayerStartLocation(0));

                    switchNextScreenBuffer();

                    story__dead = 1;
                }
                break;
            default:
                break;
        }
    }
}