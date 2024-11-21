#include "libs/manager.h"

struct Point con_size;

// extern
extern int cheat_mode;
extern int intro__dead;
extern int story__dead;

extern void secret_code_runner(int* pass, int enter, int size);
extern void intro();
extern void story();
// extern

unsigned long long start_tick;
unsigned long long now_ticks;

int exit_game = 0;

void ExitGame() {
    exit_game = 1;
}

int main() {
    setRandomSeed(time(NULL));

    char title[50] = { 0 };
    sprintf(title, "C-RPG | 알파 테스트 | v%s", game_version);

    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    SetConsoleTitle(title);
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &((SMALL_RECT) { 0, 0, console_width - 1, console_height - 1 }));
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { console_width, console_height });

    // init screen buffer
    initScreenBuffer(0);

    // get now time with milliseconds
    start_tick = GetTickCount64();

    // draw information box line
    for (int i = 0; i < getScreenBufferCount(); i++) {
        printInformationBoxLine(getScreenBuffer(i));
    }

    con_size = getConsoleSize();

    // do not change console size
    printfInInformationBox(3, "[경고] 콘솔창 크기를 변경하지 마세요. (변경시 프로그램이 종료됩니다)");
    printfInInformationBox(0, "[!] 'ENTER' 키를 누르면 이벤트를 넘길 수 있습니다.");

    int secret[10] = { 0 }; // secret code buffer
    unsigned long long before_tick = 0; // duplicate check
    while (1) { // runtime loop
        if (exit_game == 1) break;
        now_ticks = GetTickCount64() - start_tick; // get now_ticks_from_time from start

        // check console size change
        if (con_size.x != getConsoleSize().x || con_size.y != getConsoleSize().y) {
            exit(0);
        }

        if (getInput(0)) { // get key input
            secret_code_runner(secret, keyData.key, 10);

            if (canPlayerMove()) {
                switch (keyData.key) {
                    case KEY_UP:
                        movePlayer(0);
                        break;
                    case KEY_RIGHT:
                        movePlayer(1);
                        break;
                    case KEY_DOWN:
                        movePlayer(2);
                        break;
                    case KEY_LEFT:
                        movePlayer(3);
                        break;
                }
            }
        }

        // duplicate check
        if (now_ticks == before_tick) continue;
        // write after this line
        if (intro__dead != 1) intro();
        if (story__dead != 1) story();
        before_tick = now_ticks;
    }

    for (int i = 0; i < getScreenBufferCount(); i++) {
        destroyScreenBuffer(&getAllScreens()[i]);
    }

    return 0;
}

