#include "../manager.h"

struct Key keyData;

int getInput(int bypass) {
    HWND hwnd = GetConsoleWindow(); // get console window handle
    int key; // key input buffer
    int keyPressed = 0; // key pressed check

    int result = 0;

    if (!bypass && isDisabledKeyInput() == 0) {
        return result;
    }

    // get key input
    if (_kbhit()) {
        key = _getch();
        if (key == 224) { // detect movement key
            key = _getch();
            result = 1;

            keyData.key = key;
        }
        else { // detect other key
            result = 1;

            keyData.key = key;
        }
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

    return result;
}