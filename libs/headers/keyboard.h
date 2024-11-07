#pragma once

struct Key {
    int key;
    int isPressed;
};

extern struct Key keyData;

int getInput(); // 1: executed, 0: not executed