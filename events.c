extern unsigned long long now_ticks;
extern void printfXY(int x, int y, const char* format, ...);
extern void printfCenter(int y, const char* format, ...);
extern void clearConsoleLines(int start_y, int end_y);
extern int getUseableConsoleHeight();
extern void printfInInformationBox(int level, const char* format, ...);
// extern

void intro() {
    switch (now_ticks) {
        case 1000:
            printfXY(0, 0, "Welcome to C-RPG");
            printfXY(0, 1, "This is a simple RPG game made with C language.");
            break;
        case 4000:
            clearConsoleLines(0, getUseableConsoleHeight());
            printfCenter(8, "C-RPG");
            printfCenter(9, "2024.10.15");
            printfCenter(10, "Made by. R,M,C");
            break;
        case 5000:
        case 8000:
            printfCenter(30, "Loading.  ");
            break;
        case 6000:
        case 9000:
            printfCenter(30, "Loading.. ");
            break;
        case 7000:
        case 10000:
            printfCenter(30, "Loading...");
            break;
        case 11000:
            printfCenter(30, "Loading complete.");
            break;
        case 13000:
            printfCenter(31, "Press any key to start.");
            printfInInformationBox(1, "게임을 시작할 준비가 되었습니다.");
            break;
        default:
            // Handle unexpected values if necessary
            break;
    }
}