#include "libs/system.h"
#include <Windows.h>

int main() {
    system("chcp 65001");
    system("mode con cols=160 lines=43 | title \"C-RPG | Alpha Test\"");
    setColor(BLUE, BLACK);
    printfXY(20, 10, "Hello, World! %s", "김상준");
    saveConsoleLayer(0);
    Sleep(5000);

    setColor(RED, YELLOW);
    printfXY(20, 10, "NFIEUNW fwe, qinwef! %s", "fff");
    Sleep(5000);

    printConsoleLayer(0);
    Sleep(5000);
}