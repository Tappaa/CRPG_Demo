#include <conio.h>
#include "libs/allinone.h"

struct Point con_size;

// extern
extern int cheat_mode;
extern int intro__dead;
extern int start_screen_button__dead;
extern int story__dead;

extern void secret_code_runner(int* pass, int enter, int size);
extern void intro();
extern void start_screen_button();
extern void story();
// extern

unsigned long long start_tick;
unsigned long long now_ticks;

int exit_game = 0;

struct Key keyData;

void ExitGame() {
    exit_game = 1;
}

int main() {
    // get console window handle
    HWND hwnd = GetConsoleWindow();

    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    system("mode con cols=160 lines=43 | title \"C-RPG | Alpha Test\"");

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

    int key; // key input buffer
    int keyPressed = 0; // key pressed check
    int secret[10] = { 0 }; // secret code buffer
    unsigned long long before_tick = 0; // duplicate check
    while (1) { // runtime loop
        if (exit_game == 1) break;
        now_ticks = GetTickCount64() - start_tick; // get now_ticks_from_time from start

        // check console size change
        if (con_size.x != getConsoleSize().x || con_size.y != getConsoleSize().y) {
            exit(0);
        }

        if (isDisabledKeyInput() == 0) {
            // get key input
            if (_kbhit()) {
                key = _getch();
                if (key == 224) { // detect movement key
                    key = _getch();
                    secret_code_runner(secret, key, sizeof(secret) / sizeof(int));

                    keyData.key = key;
                }
                else { // detect other key
                    secret_code_runner(secret, key, sizeof(secret) / sizeof(int));

                    keyData.key = key;
                }
            } else {
            }

            // check is not background
            if (GetForegroundWindow() == hwnd) {
                // check key pressed
                int before = keyPressed;
                for (key = 0; key < 256; key++) {
                    // ignore mouse buttons
                    if (key == VK_LBUTTON || key == VK_RBUTTON || key == VK_MBUTTON ||
                        key == VK_XBUTTON1 || key == VK_XBUTTON2) {
                        continue;
                    }

                    if (GetAsyncKeyState(key) & 0x8000) {
                        keyPressed += 1;
                        // prevent overflow
                        keyPressed %= 2;
                    }
                }

                if (before != keyPressed) {
                    keyData.isPressed = 1;
                } else {
                    keyData.isPressed = 0;
                }
            }
        }

        // duplicate check
        if (now_ticks == before_tick) continue;
        // write after this line
        if (intro__dead != 1) intro();
        if (start_screen_button__dead != 1) start_screen_button();
        if (story__dead != 1) story();
        before_tick = now_ticks;
    }

    for (int i = 0; i < getScreenBufferCount(); i++) {
        destroyScreenBuffer(&getAllScreens()[i]);
    }

    return 0;
}

