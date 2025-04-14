//#include <iostream>
//#include <Windows.h>
//#include <conio.h>
//
//using namespace std;
//
//void GotoXY(int X, int Y)
//{
//    COORD Cur;
//    Cur.X = X;
//    Cur.Y = Y;
//    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
//}
//
//void DrawWall(int left, int top, int width, int height)
//{
//    for (int y = top; y < top + height; ++y)
//    {
//        for (int x = left; x < left + width; ++x)
//        {
//            if (y == top || y == top + height - 1 || x == left || x == left + width - 1)
//            {
//                GotoXY(x, y);
//                cout << "*";
//            }
//        }
//    }
//}
//
//struct PlayerInfo
//{
//    int X;
//    int Y;
//    string Shape;
//};
//
//const int WALL_LEFT = 0;
//const int WALL_TOP = 0;
//const int WALL_WIDTH = 10;
//const int WALL_HEIGHT = 10;
//
//PlayerInfo* PlayerData = nullptr;
//int InputKey = 0;
//
//bool IsRunning = true;
//
//void Input() // 게임 입력 처리
//{
//    InputKey = _getch();
//    if (InputKey == 27) // ESC
//        IsRunning = false;
//}
//
//
//void Tick() // 게임 로직 처리(이동 범위 제한)
//{
//    // 10(0~9) * 10(0~9) 박스에서 플레이어는 8(1~8) * 8(1~8)만 이동 가능
//    // 플레이어가 (1,1)일 때는 1 > 1이 성립하지 않으므로 이동 불가능.  
//    if ((InputKey == 0x48 || toupper(InputKey) == 'W') && PlayerData->Y > WALL_TOP + 1)
//    {
//        PlayerData->Y--; //위 방향키 입력 또는 W 입력이면서 Y축이 1보다 클 때 위로 이동 가능
//    }
//    else if ((InputKey == 0x4B || toupper(InputKey) == 'A') && PlayerData->X > WALL_LEFT + 1)
//    {
//        PlayerData->X--; //왼쪽 방향키 입력 또는 A 입력이면서 X축이 1보다 클 때 위로 이동 가능
//    }
//    else if ((InputKey == 0x4D || toupper(InputKey) == 'D') && PlayerData->X < WALL_LEFT + WALL_WIDTH - 2)
//    {
//        PlayerData->X++; //오른쪽 방향키 입력 또는 A 입력이면서 X축이 8보다 작을 때 오른쪽으로 이동 가능
//    }
//    else if ((InputKey == 0x50 || toupper(InputKey) == 'S') && PlayerData->Y < WALL_TOP + WALL_HEIGHT - 2)
//    {
//        PlayerData->Y++; //아래 방향키 입력 또는 S 입력이면서 Y축이 8보다 작을 때 아래로 이동 가능
//    }
//}
//
//void Render() // 화면 출력(플레이어와 벽 표시)
//{
//    system("cls");
//    DrawWall(WALL_LEFT, WALL_TOP, WALL_WIDTH, WALL_HEIGHT);
//    GotoXY(PlayerData->X, PlayerData->Y);
//    cout << PlayerData->Shape;
//}
//
//int main()
//{
//    PlayerData = new PlayerInfo;
//    PlayerData->X = 1;
//    PlayerData->Y = 1;
//    PlayerData->Shape = "P";
//
//    while (IsRunning)
//    {
//        Input();
//        Tick();
//        Render();
//    }
//
//    delete PlayerData;
//    PlayerData = nullptr;
//
//    return 0;
//}


#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

struct ActorInfo
{
	int X;
	int Y;
	char Shape;
};


// 전역변수
int MapHeight = 10;
int KeyCode = 0;
ActorInfo* Player = nullptr;
ActorInfo* Monster = nullptr;
ActorInfo* Goal = nullptr;

string* Map;
bool IsRunning = true;



void Initialize();
void Input();
void Tick();
void Render();
void GotoXY(int x, int y);
void Terminate();
bool Predict(int PredictX, int PredictY);

int main()
{
	Initialize();

	while (IsRunning)  //frame, game loop
	{
		Input();
		Tick();
		Render();
	}

	Terminate();

	return 0;
}

void Initialize()
{
	//Load
	Map = new string[MapHeight];
	//FILE
	Map[0] = "***************";
	Map[1] = "*        ******";
	Map[2] = "*        *** **";
	Map[3] = "*        **  **";
	Map[4] = "****     **  **";
	Map[5] = "*        *  ***";
	Map[6] = "*          ****";
	Map[7] = "*   *    ******";
	Map[8] = "*   *    ******";
	Map[9] = "***************";

	Player = new ActorInfo();
	Player->X = 1;
	Player->Y = 1;
	Player->Shape = 'P';

	Monster = new ActorInfo();
	Monster->X = 8;
	Monster->Y = 8;
	Monster->Shape = 'M';

	Goal = new ActorInfo();
	Goal->X = 5;
	Goal->Y = 5;
	Goal->Shape = 'G';

	srand((unsigned int)(time(nullptr)));

}

void Input()
{
	KeyCode = _getch(); //Device
}

void Tick()
{
	//Player
	if (KeyCode == 'w' && Predict(Player->X, Player->Y - 1))
	{
		Player->Y--;
	}
	if (KeyCode == 's' && Predict(Player->X, Player->Y + 1))
	{
		Player->Y++;
	}
	if (KeyCode == 'a' && Predict(Player->X - 1, Player->Y))
	{
		Player->X--;
	}
	if (KeyCode == 'd' && Predict(Player->X + 1, Player->Y))
	{
		Player->X++;
	}

	if (KeyCode == 'q')
	{
		IsRunning = false;
	}

	//AI(Random, HP, Attack, BT, FSM, Tick )
	//Monster
	int Direction = rand() % 4;
	if (Direction == 0)
	{
		if (Predict(Monster->X, Monster->Y - 1))
		{
			Monster->Y--;
		}
	}
	if (Direction == 1)
	{
		if (Predict(Monster->X, Monster->Y + 1))
		{
			Monster->Y++;
		}
	}
	if (Direction == 2)
	{
		if (Predict(Monster->X - 1, Monster->Y))
		{
			Monster->X--;
		}
	}
	if (Direction == 3)
	{
		if (Predict(Monster->X + 1, Monster->Y))
		{
			Monster->X++;
		}
	}

	if (Player->X == Goal->X && Player->Y == Goal->Y)
	{
		IsRunning = false;
	}
}

void Render()
{
	//system("cls");
	for (int Y = 0; Y < MapHeight; ++Y)
	{
		for (int X = 0; X < Map[Y].length(); ++X)
		{
			GotoXY(X, Y);
			cout << Map[Y][X];
		}
	}

	void GoalIn();

	//Actor, Pawn
	GotoXY(Player->X, Player->Y);
	cout << Player->Shape;

	GotoXY(Monster->X, Monster->Y);
	cout << Monster->Shape;

	GotoXY(Goal->X, Goal->Y);
	cout << Goal->Shape;
}

void GotoXY(int X, int Y)
{
	COORD Pos;
	Pos.X = X;
	Pos.Y = Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void Terminate()
{
	delete Player;
	Player = nullptr;

	delete Monster;
	Monster = nullptr;

	delete Goal;
	Goal = nullptr;

	delete[] Map;
	Map = nullptr;
}

bool Predict(int PredictX, int PredictY)
{
	if (Map[PredictY][PredictX] == ' ')
	{
		return true;
	}

	return false;
}
