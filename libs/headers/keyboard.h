#pragma once

/* in game inputs */
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_A 65
#define KEY_B 66
#define KEY_CHEAT_MODE 92
#define KEY_NULL 0
#define KEY_ESCAPE 27
/* in game inputs */

struct Key {
    int key;
    int isPressed;
};

extern struct Key keyData;

int getInput(int bypass); // 1: executed, 0: not executed | bypass: 1: bypass, 0: not bypass