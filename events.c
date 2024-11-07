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
    switch (now_ticks) {
        case 1000:
            printfXY(getCurrentScreenBuffer(), 0, 0, "Welcome to C-RPG");
            printfXY(getCurrentScreenBuffer(), 0, 1, "This is a simple RPG game made with C language.");
            break;
        case 4000:
            clearConsoleLines(getCurrentScreenBuffer(), 0, getAvailableConsoleHeight());
            printfCenter(getCurrentScreenBuffer(), 8, "C-RPG");
            printfCenter(getCurrentScreenBuffer(), 9, "2024.10.15");
            printfCenter(getCurrentScreenBuffer(), 10, "Made by. R,M,C");
            printfCenter(getCurrentScreenBuffer(), 11, "Version. %s", game_version);
            copyScreenBuffer(getCurrentScreenBufferIndex(), getNextScreenBufferIndex());
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
            switchNextScreenBuffer();
            break;
        case 13000:
            printfCenter(getCurrentScreenBuffer(), 31, "Press any key to continue.");
            printfInInformationBox(1, "게임을 시작할 준비가 되었습니다.");
            break;
        default:
            if (now_ticks > 13000) {
                printContinueAction(31);

                intro__dead = 1;
                clearConsoleLines(getNextScreenBuffer(), 20, getAvailableConsoleHeight());

                // draw buttons
                struct Point start = { 72, 30 };
                struct Point end = { 85, 33 };
                printEdgeLines(getNextScreenBuffer(), start, end);
                setColor(getNextScreenBuffer(), WHITE, BLACK);
                printfCenter(getNextScreenBuffer(), 31, "게임 시작");
                setColor(getNextScreenBuffer(), BLACK, WHITE);
                printfCenter(getNextScreenBuffer(), 32, "게임 종료");
                copyScreenBuffer(getCurrentScreenBufferIndex(), getNextScreenBufferIndex());
                switchNextScreenBuffer();
            }
            break;
    }
}

int start_screen_button__selected = 0;
int start_screen_button__selected__local_ticks = 16;
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
            printfCenter(getCurrentScreenBuffer(), 32, "게임 종료");
            setColor(getCurrentScreenBuffer(), WHITE, BLACK);
            printfCenter(getCurrentScreenBuffer(), 31, "게임 시작");
            resetColor(getCurrentScreenBuffer());
        } else if (start_screen_button__selected == 1) {
            printfCenter(getCurrentScreenBuffer(), 31, "게임 시작");
            setColor(getCurrentScreenBuffer(), WHITE, BLACK);
            printfCenter(getCurrentScreenBuffer(), 32, "게임 종료");
            resetColor(getCurrentScreenBuffer());
        }

        // button click
        if (keyData.key == KEY_ENTER && keyData.isPressed == 1 && start_screen_button__selected__local_ticks == 0) {
            if (start_screen_button__selected == 0) {
                printfInInformationBox(1, "게임을 시작합니다.");

                start_screen_button__dead = 1;
                clearScreenBuffer(getNextScreenBufferIndex());
                switchNextScreenBuffer();
            } else if (start_screen_button__selected == 1) {
                printfInInformationBox(1, "게임을 종료합니다.");
                ExitGame();
            }
        }
    }
}

int story__local_ticks = 0;
int story__page = 0;
int story__dead = 0;
void story() {
    if (start_screen_button__dead == 1) {
        story__local_ticks++;

        switch (story__page) {
            case 0:
                if (story__local_ticks == 64) {
                    clearScreenBuffer(getNextScreenBufferIndex());
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
                    clearScreenBuffer(getNextScreenBufferIndex());
                    switchNextScreenBuffer();
                }
                break;
            case 1:
                if (story__local_ticks == 64) {
                    clearScreenBuffer(getNextScreenBufferIndex());
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
                    clearScreenBuffer(getNextScreenBufferIndex());

                    initPlayer();
                    createPlayer((struct Point) { 1, 1 });

                    int length = asciiArtLength(getSlimeStats());
                    for (int i = 0; i < length; i++) {
                        printfXY(getNextScreenBuffer(), 5, 1 + i, getSlimeStats().ascii_art[i]);
                    }

//                    struct Point rb = { getConsoleSize().x - 1, getAvailableConsoleHeight() + 1 };
//                    printEdgeLines(getNextScreenBuffer(), zz, rb);
//
//                    switchNextScreenBuffer();
//
//                    initPlayer();
//
//                    int try = 0;
//                    while (1) {
//                        if (criticalCheck()) {
//                            printfInInformationBox(2, "크리티컬!");
//                            break;
//                        } else {
//                            printfInInformationBox(2, "ㄲㅂ %d", try);
//                            try++;
//                        }
//                        Sleep(500);
//                    }
//                    playerDead("멍청이");
                }
                break;
            default:
                break;
        }
    }
}