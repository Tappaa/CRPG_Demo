#include "../manager.h"

struct Key keyData;

int getInput() {
    HWND hwnd = GetConsoleWindow(); // get console window handle
    int key; // key input buffer
    int keyPressed = 0; // key pressed check

    int ret = 0;

    if (isDisabledKeyInput() == 0) {
        // get key input
        if (_kbhit()) {
            key = _getch();
            if (key == 224) { // detect movement key
                key = _getch();
                ret = 1;

                keyData.key = key;
            }
            else { // detect other key
                ret = 1;

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
    }

    return ret;
}