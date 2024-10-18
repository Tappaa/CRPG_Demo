#include <stdio.h>
#include <conio.h>
#include <Windows.h>

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
/* in game inputs */

struct Point {
    int x;
    int y;
};

int cheat_mode = 0;
unsigned long long start_tick;
unsigned long long now_ticks;

struct Point con_size;

enum ColorType{
    BLACK,  	//0
    darkBLUE,	//1
    darkGreen,	//2
    darkSkyBlue,    //3
    darkRed,  	//4
    darkPurple,	//5
    darkYellow,	//6
    GRAY,		//7
    darkGray,	//8
    BLUE,		//9
    GREEN,		//10
    SkyBlue,	//11
    RED,		//12
    PURPLE,		//13
    YELLOW,		//14
    WHITE		//15
} COLOR;

/* basic console functions */

void gotoXY(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

struct Point getConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    struct Point result = { csbi.srWindow.Right - csbi.srWindow.Left, csbi.srWindow.Bottom - csbi.srWindow.Top };
    return result;
}

struct Point getCursorPos() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    struct Point result = { csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y };
    return result;
}

void setTextColor(int colorNum) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

/* basic console functions */

int getUseableConsoleHeight() {
    struct Point size = getConsoleSize();
    return size.y - 3;
}

void printfCenter(int y, char* str, ...) {
    struct Point size = getConsoleSize();
    va_list args;
    va_start(args, str);

    int len = vsnprintf(NULL, 0, str, args);
    int x = (size.x - len) / 2;
    gotoXY(x, y);
    vprintf(str, args);

    va_end(args);
}

void printfXY(int x, int y, char* str, ...) {
    gotoXY(x, y);
    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);
}

void clearConsoleLines(int start_y, int end_y) {
    struct Point size = getConsoleSize();
    if (end_y == -1 || end_y > size.y) end_y = size.y;
    for (int y = start_y; y <= end_y; y++) {
        gotoXY(0, y);
        for (int x = 0; x <= size.x; x++) {
            printf(" ");
        }
    }
}

int past_level = 0; // 0 : 하얀색 1 : 파란색 3 : 노란색 4 : 빨간색
char past_str[250] = ""; // 250 is enough
void printfInInformationBox(int level, char *str, ...) {
    struct Point size = getConsoleSize();
    int y = size.y;

    // clear information box
    clearConsoleLines(y - 1, -1);

    // past string
    if (strcmp(past_str, "") != 0) {
        switch (past_level) {
            case 0:
                setTextColor(WHITE);
                break;
            case 1:
                setTextColor(BLUE);
                break;
            case 2:
                setTextColor(YELLOW);
                break;
            case 3:
                setTextColor(RED);
                break;
            default:
                break;
        }
        printfXY(0, y - 1, past_str);
        setTextColor(WHITE);
    }
    past_level = level;

    switch (level) {
        case 0:
            setTextColor(WHITE);
            break;
        case 1:
            setTextColor(BLUE);
            break;
        case 2:
            setTextColor(YELLOW);
            break;
        case 3:
            setTextColor(RED);
            break;
        default:
            break;
    }
    gotoXY(0, y);
    va_list args;
    va_start(args, str);
    vsnprintf(past_str, sizeof(past_str), str, args);
    vprintf(str, args);
    va_end(args);
    setTextColor(WHITE);
}

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
//        printf("%d\t", pass[i]);
    }
    //printf("\n");
    // U D U D L R L R B A
    // A B R L R L D U D U
    if (pass[0] == KEY_A && pass[1] == KEY_B && pass[2] == KEY_RIGHT && pass[3] == KEY_LEFT && pass[4] == KEY_RIGHT && pass[5] == KEY_LEFT && pass[6] == KEY_DOWN && pass[7] == KEY_UP && pass[8] == KEY_DOWN && pass[9] == KEY_UP) {
        cheat_mode = 1;
        printfInInformationBox(0, "[!] 치트모드가 활성화 되었습니다. [%d]", cheat_mode);
    }
}

extern void intro();
int main() {
    // get now time with milliseconds
    start_tick = GetTickCount64();
    system("chcp 65001");
    system("mode con cols=160 lines=44 | title \"C-RPG | FPS : 999\"");
    for (int i = 0; i <= getConsoleSize().x; i++) {
        gotoXY(i, getUseableConsoleHeight() + 1);
        printf("-");
    }
    con_size = getConsoleSize();

    // do not change console size
    printfInInformationBox(3, "[경고] 콘솔창 크기를 변경하지 마세요. (변경시 프로그램이 종료됩니다)");

    int key;
    int secret[10] = { 0 };
    unsigned long long before_tick = 0;
    while (1) { // runtime loop
        now_ticks = GetTickCount64() - start_tick; // get now_ticks_from_time from start

        // check console size change
        if (con_size.x != getConsoleSize().x || con_size.y != getConsoleSize().y) {
            exit(0);
        }

        if (_kbhit()) {
            key = _getch();
            if (key == 224) { // detect movement key
                key = _getch();
                secret_code_runner(secret, key, sizeof(secret) / sizeof(int));
                switch (key) {
                    case KEY_LEFT:
//                        printf("%d, 왼쪽으로 이동\n", key);
                        break;
                    case KEY_RIGHT:
//                        printf("%d, 오른쪽으로 이동\n", key);
                        break;
                    case KEY_UP:
//                        printf("%d, 위로 이동\n", key);
                        break;
                    case KEY_DOWN:
//                        printf("%d, 아래로 이동\n", key);
                        break;
                    default:
                        break;
                }
            }
            else { // detect other key
                secret_code_runner(secret, key, sizeof(secret) / sizeof(int));
//                printf("%d, %c 입력\n", key, key);
            }
        }

        // duplicate check
        if (now_ticks == before_tick) continue;
        // write after this line
        intro();
        before_tick = now_ticks;
    }
}

