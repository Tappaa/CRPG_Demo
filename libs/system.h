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
};

void gotoXY(int x, int y);
struct Point getConsoleSize();
struct Point getCursorPos();
void setTextColor(int colorNum);
int getUseableConsoleHeight();
void printfCenter(int y, char* str, ...);
void printfXY(int x, int y, char* str, ...);
void clearConsoleLines(int start_y, int end_y);
void printfInInformationBox(int level, char *str, ...); // level : 0 : 하얀색 1 : 파란색 3 : 노란색 4 : 빨간색
