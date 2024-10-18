#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

/* ingame inputs */
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_A 65
#define KEY_B 66
#define KEY_CHEAT_MODE 92
/* ingame inputs */

int cheat_mode = 0;
int start_tick;

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
        //printf("%d\t", pass[i]);
    }
    //printf("\n");
    // U D U D L R L R B A
    // A B R L R L D U D U
    if (pass[0] == KEY_A && pass[1] == KEY_B && pass[2] == KEY_RIGHT && pass[3] == KEY_LEFT && pass[4] == KEY_RIGHT && pass[5] == KEY_LEFT && pass[6] == KEY_DOWN && pass[7] == KEY_UP && pass[8] == KEY_DOWN && pass[9] == KEY_UP) {
        cheat_mode = 1;
        printf("\n치트코드 감지.");
    }
}

int main() {
    start_tick = time(NULL);
    system("mode con cols=160 lines=40 | title \"C-RPG | FPS : 999\"");

    char key;
    int secret[10] = { 0 };
    int ticks = 0;
    while (1) { // runtime
        if (_kbhit()) {
            key = _getch();
            if (key == -32) { // detect movement key
                key = _getch();
                secret_code_runner(secret, key, sizeof(secret) / sizeof(int));
                switch (key) {
                    case KEY_LEFT:
                        //printf("%d, 왼쪽으로 이동\n", key);
                        break;
                    case KEY_RIGHT:
                        //printf("%d, 오른쪽으로 이동\n", key);
                        break;
                    case KEY_UP:
                        //printf("%d, 위로 이동\n", key);
                        break;
                    case KEY_DOWN:
                        //printf("%d, 아래로 이동\n", key);
                        break;
                }
            }
            else { // detect other key
                secret_code_runner(secret, key, sizeof(secret) / sizeof(int));
                //printf("%d, %c 입력\n", key, key);
            }
        }
    }
}

