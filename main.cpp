#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

void GotoXY(int X, int Y)
{
    COORD Cur;
    Cur.X = X;
    Cur.Y = Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void DrawWall(int left, int top, int width, int height)
{
    for (int y = top; y < top + height; ++y)
    {
        for (int x = left; x < left + width; ++x)
        {
            if (y == top || y == top + height - 1 || x == left || x == left + width - 1)
            {
                GotoXY(x, y);
                cout << "*";
            }
        }
    }
}

struct PlayerInfo
{
    int X;
    int Y;
    string Shape;
};

const int WALL_LEFT = 0;
const int WALL_TOP = 0;
const int WALL_WIDTH = 10;
const int WALL_HEIGHT = 10;

PlayerInfo* PlayerData = nullptr;
int InputKey = 0;

bool IsRunning = true;

void Input()
{
    InputKey = _getch();
    if (InputKey == 27) // ESC
        IsRunning = false;
}


void Tick()
{
    // 10(0~9) * 10(0~9) 박스에서 플레이어는 8(1~8) * 8(1~8)만 이동 가능
    if ((InputKey == 0x48 || toupper(InputKey) == 'W') && PlayerData->Y > WALL_TOP + 1)
    {
        PlayerData->Y--; //위 방향키 입력 또는 W 입력이면서 Y축이 1보다 클 때 위로 이동 가능
    }
    else if ((InputKey == 0x4B || toupper(InputKey) == 'A') && PlayerData->X > WALL_LEFT + 1)
    {
        PlayerData->X--; //왼쪽 방향키 입력 또는 A 입력이면서 X축이 1보다 클 때 위로 이동 가능
    }
    else if ((InputKey == 0x4D || toupper(InputKey) == 'D') && PlayerData->X < WALL_LEFT + WALL_WIDTH - 2)
    {
        PlayerData->X++; //오른쪽 방향키 입력 또는 A 입력이면서 X축이 8보다 작을 때 오른쪽으로 이동 가능
    }
    else if ((InputKey == 0x50 || toupper(InputKey) == 'S') && PlayerData->Y < WALL_TOP + WALL_HEIGHT - 2)
    {
        PlayerData->Y++; //아래 방향키 입력 또는 S 입력이면서 Y축이 8보다 작을 때 아래로 이동 가능
    }
}

void Render()
{
    system("cls");
    DrawWall(WALL_LEFT, WALL_TOP, WALL_WIDTH, WALL_HEIGHT);
    GotoXY(PlayerData->X, PlayerData->Y);
    cout << PlayerData->Shape;
}

int main()
{
    PlayerData = new PlayerInfo;
    PlayerData->X = 1;
    PlayerData->Y = 1;
    PlayerData->Shape = "P";

    while (IsRunning)
    {
        Input();
        Tick();
        Render();
    }

    delete PlayerData;
    PlayerData = nullptr;

    return 0;
}
