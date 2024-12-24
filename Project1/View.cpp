#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <SFML/Audio.hpp>
#include <thread>
using namespace std;

#define BOARD_SIZE 14 //Kích thước ma trận bàn cờ
#define LEFT 32 //Tọa độ trái màn hình bàn cờ
#define TOP 1 //Tọa độ trên màn hình bàn cờ

extern int _A[BOARD_SIZE][BOARD_SIZE]; //Ma trận bàn cờ
extern int _BOARD_X, _BOARD_Y; //Biến nhận giá trị là tọa độ của bàn cờ
extern int _TURN; //-1 là lượt người thứ nhất và 1 là lượt người thứ hai
extern int _COMMAND; //Biến nhận giá trị phím người dùng nhập
extern int _X, _Y; //Tọa độ hiện hành trên màn hình bàn cờ
extern int _SCREEN; //Biến hiển thị đang ở màn hình menu nào
extern int _SCREEN_OPTION[4]; //Số lựa chọn trong mỗi màn hình menu
extern int _OPTION; //Thứ tự hộp menu
extern int Score1;
extern int Score2;
extern int Step1;
extern int Step2;
extern int isLoadGame; //Biến nhận giá trị có tải game đã lưu không
extern string Player1_name;
extern string Player2_name;
extern string FileName[10]; //Tên các game đã lưu
extern string FileNameEnter; //Biến nhận tên game mà người dùng nhập
extern string Music;
extern string Sound_effect;

void FixConsoleWindow();
void DeleteConsole();
void GotoXY(int x, int y);
void SetColor(int backgound_color, int text_color);
void DrawCARO(int x, int y); //Vẽ chữ CARO ở màn hình menu
void DrawBox(int x, int y, int width, int height);
void TextBox(int x, int y, int width, string s); //Căn cho chữ ở giữa ô với độ rộng width
void BackgroundColor(int x, int y, int width, int height, int color);
void DrawBoard(int x, int y, int width_size, int height_size);
void DrawMenuBackground(); //Vẽ giao diện menu không có các option
void DrawNewGame(int background_color); //Vẽ hộp option có chữ New Game
void DrawLoadGame(int background_color);  //Vẽ hộp option có chữ Load Game
void DrawSettings(int background_color); //Vẽ hộp option có chữ Settings
void DrawHelp(int background_color); //Vẽ hộp option có chữ Help
void DrawExit(int background_color); //Vẽ hộp option có chữ Exit
void DrawMusic(int background_color); //Vẽ hộp option có chữ Music: ON/OFF
void DrawSoundEffect(int background_color); //Vẽ hộp option có chữ Sound Efect: ON/OFF
void Draw1Player(int background_color); //Vẽ hộp option có chữ 1 Player
void Draw1PlayerOption(); //Màn hình sau khi nhấn 1 Player
void Draw2Player(int background_color); //Vẽ hộp option có chữ 2 Player
void Draw2PlayerOption(); //Màn hình sau khi nhấn 2 Player
void DrawMenu(); //Vẽ menu ban đầu
void DrawBack(); //Vẽ hộp option có chữ B:Back
void DrawHelpOption(); //Màn hình sau khi nhấn Help
void DrawNewGameOption(); //Màn hình sau khi nhấn New Game
void DrawLoadGameOption(); //Màn hình sau khi nhấn Load Game
void DrawSaveGameOption(); //Màn hình sau khi nhấn Save Game
void DrawSettingsOption(); //Màn hình sau khi nhấn Settings
void DrawWin(int x, int y, int color); //Vẽ chữ Win
void DrawDraw(int color); //Vẽ chữ Draw
void DrawX(); //Vẽ chữ X
void DrawO(); //Vẽ chữ O
void DeleteX(); //Xóa chữ X
void DeleteO(); //Xóa chữ O
void WinEffect(int player_win); //Hiệu ứng khi tháng
void DrawEffect(); //Hiệu ứng khi hòa
void AskContinue();
void DrawPvP();
void DrawPvC();

//Model.cpp
void PlaySoundEffect(const string& filename);


void FixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void DeleteConsole()
{
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}


void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int backgound_color, int text_color)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	int color_code = backgound_color * 16 + text_color;
	SetConsoleTextAttribute(hStdout, color_code);
}

void DrawCARO(int x, int y)
{
	int r[5][66] = { {1,2,3,4,9,9,9,9,9,9,9,9,9,9,15,16,17,18,19,20,21,22,9,9,9,9,9,9,9,9,31,32,33,34,35,36,9,9,9,9,9,9,9,9,9,9,15,15,15,15,15,15,15,15,9,9,9,9,9,9,9,9,15,15,1,1},
					 {1,2,9,9,9,9,7,8,8,10,11,12,9,9,9,9,17,18,19,20,9,9,9,9,25,26,27,28,9,9,9,9,33,34,35,36,9,9,9,9,41,42,43,15,9,9,9,15,15,15,15,15,9,9,9,9,15,15,15,15,9,9,9,9,1,1},
					 {1,2,9,9,5,6,7,8,8,10,11,12,13,14,15,16,17,18,19,20,9,9,9,9,9,9,9,9,9,9,9,9,33,34,35,36,9,9,9,9,9,9,9,9,9,9,15,15,15,15,15,15,9,9,9,9,15,15,15,15,9,9,9,9,1,1},
					 {1,2,9,9,9,9,7,8,8,10,11,12,9,9,9,9,17,18,19,20,9,9,9,9,25,26,27,28,9,9,9,9,33,34,35,36,9,9,9,9,41,42,43,9,9,9,9,15,15,15,15,15,9,9,9,9,15,15,15,15,9,9,9,9,1,1},
					 {1,2,3,4,9,9,9,9,9,9,9,9,9,9,15,16,17,18,19,20,9,9,9,9,25,26,27,28,9,9,9,9,33,34,35,36,9,9,9,9,41,42,43,15,9,9,9,9,15,15,15,15,15,15,9,9,9,9,9,9,9,9,15,15,1,1} };
	SetColor(9, 0);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 66; j++)
		{
			if (r[i][j] == 9)
			{
				GotoXY(x + j, y + i);
				cout << " ";
			}
		}
	}
	SetColor(15, 0);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 66; j++)
		{
			if (r[i][j] != 9)
			{
				GotoXY(x + j, y + i);
				cout << " ";
			}
		}
	}
}

void DrawBox(int x, int y, int width, int height)
{
	GotoXY(x, y);
	cout << char(201);
	GotoXY(x, y + height + 1);
	cout << char(200);
	GotoXY(x + width + 1, y);
	cout << char(187);
	GotoXY(x + width + 1, y + height + 1);
	cout << char(188);
	for (int i = 1; i <= width; i++)
	{
		GotoXY(x + i, y);
		cout << char(205);
		GotoXY(x + i, y + height + 1);
		cout << char(205);
	}
	for (int j = 1; j <= height; j++)
	{
		GotoXY(x, y + j);
		cout << char(186);
		GotoXY(x + width + 1, y + j);
		cout << char(186);
	}
}

void TextBox(int x, int y, int width, string s)
{
	GotoXY(x + (width - s.length()) / 2, y);
	cout << s;
}

void BackgroundColor(int x, int y, int width, int height, int color)
{
	SetColor(color, color);
	for (int i = x; i < x + width; i++)
		for (int j = y; j < y + height; j++)
		{
			GotoXY(i, j);
			cout << " ";
		}
}

void DrawBoard(int x, int y, int width_size, int height_size)
{
	GotoXY(x, y);
	cout << char(218);
	for (int i = 1; i < width_size; i++)
	{
		cout << char(196) << char(196) << char(196) << char(194);
	}
	cout << char(196) << char(196) << char(196) << char(191);
	for (int i = 1; i < height_size; i++)
	{
		GotoXY(x, y + 2 * i - 1);
		cout << char(179);
		for (int j = 1; j < width_size; j++)
		{
			cout << "   " << char(179);
		}
		cout << "   " << char(179);
		GotoXY(x, y + 2 * i);
		cout << char(195);
		for (int j = 1; j < width_size; j++)
		{
			cout << char(196) << char(196) << char(196) << char(197);
		}
		cout << char(196) << char(196) << char(196) << char(180);
	}
	GotoXY(x, y + 2 * height_size - 1);
	cout << char(179);
	for (int j = 1; j < width_size; j++)
	{
		cout << "   " << char(179);
	}
	cout << "   " << char(179);
	GotoXY(x, y + 2 * height_size);
	cout << char(192);
	for (int j = 1; j < width_size; j++)
	{
		cout << char(196) << char(196) << char(196) << char(193);
	}
	cout << char(196) << char(196) << char(196) << char(217);
}

void DrawMenuBackground()
{
	BackgroundColor(0, 0, 120, 30, 15);
	SetColor(15, 7);
	DrawBoard(0, 0, 29, 14);
	GotoXY(0, 0);
	cout << char(197);
	for (int i = 1; i < 30; i++)
	{
		cout << char(196) << char(196) << char(196) << char(197);
	}
	for (int i = 1; i < 15; i++)
	{
		GotoXY(0, 2 * i);
		cout << char(197);
	}
	GotoXY(1, 28);
	for (int i = 1; i < 30; i++)
	{
		cout << char(196) << char(196) << char(196) << char(197);
	}
	GotoXY(117, 0);
	cout << char(196) << char(196) << char(196);
	for (int i = 1; i < 15; i++)
	{
		GotoXY(116, 2 * i);
		cout << char(197) << char(196) << char(196) << char(196);
	}
	GotoXY(0, 29);
	cout << char(179);
	for (int i = 1; i < 30; i++)
	{
		cout << "   " << char(179);
	}
	DrawCARO(29, 3);
}

void DrawNewGame(int background_color)
{
	BackgroundColor(51, 15, 18, 1, background_color);
	SetColor(background_color, 0);
	DrawBox(50, 14, 18, 1);
	TextBox(51, 15, 18, "NEW GAME");
}

void DrawLoadGame(int background_color)
{
	BackgroundColor(51, 18, 18, 1, background_color);
	SetColor(background_color, 0);
	DrawBox(50, 17, 18, 1);
	TextBox(51, 18, 18, "LOAD GAME");
}

void DrawSettings(int background_color)
{
	BackgroundColor(51, 21, 18, 1, background_color);
	SetColor(background_color, 0);
	DrawBox(50, 20, 18, 1);
	TextBox(51, 21, 18, "SETTINGS");
}

void DrawHelp(int background_color)
{
	BackgroundColor(51, 24, 18, 1, background_color);
	SetColor(background_color, 0);
	DrawBox(50, 23, 18, 1);
	TextBox(51, 24, 18, "HELP");
}

void DrawExit(int background_color)
{
	BackgroundColor(51, 27, 18, 1, background_color);
	SetColor(background_color, 0);
	DrawBox(50, 26, 18, 1);
	TextBox(51, 27, 18, "EXIT");
}

void DrawMusic(int background_color)
{
	BackgroundColor(51, 15, 18, 1, background_color);
	SetColor(background_color, 0);
	DrawBox(50, 14, 18, 1);
	GotoXY(55, 15);
	cout << "MUSIC: " << Music;
}

void DrawSoundEffect(int background_color)
{
	BackgroundColor(51, 18, 18, 1, background_color);
	SetColor(background_color, 0);
	DrawBox(50, 17, 18, 1);
	GotoXY(52, 18);
	cout << "SOUND EFFECT: " << Sound_effect;
}

void Draw1Player(int background_color)
{
	BackgroundColor(51, 15, 18, 1, background_color);
	SetColor(background_color, 0);
	DrawBox(50, 14, 18, 1);
	TextBox(51, 15, 18, "     1 PLAYERS    ");
}

void Draw1PlayerOption()
{
	DrawMenuBackground();
	GotoXY(30, 15);
	cout << "Player's Name (maximum 14 characters): ";
	getline(cin, Player1_name);
	if (Sound_effect == "ON")
	{
		thread clickThread(PlaySoundEffect, "click.wav");
		clickThread.detach();
	}
	if (Player1_name.length() > 14)
	{
		Player1_name = Player1_name.substr(0, 14);
	}
}

void Draw2Player(int background_color)
{
	BackgroundColor(51, 18, 18, 1, background_color);
	SetColor(background_color, 0);
	DrawBox(50, 17, 18, 1);
	TextBox(51, 18, 18, "     2 PLAYERS    ");
}

void Draw2PlayerOption()
{
	DrawMenuBackground();
	GotoXY(30, 15);
	cout << "Player 1's Name (maximum 14 characters): ";
	getline(cin, Player1_name);
	if (Sound_effect == "ON")
	{
		thread clickThread(PlaySoundEffect, "click.wav");
		clickThread.detach();
	}
	GotoXY(30, 17);
	cout << "Player 2's Name (maximum 14 characters): ";
	getline(cin, Player2_name);
	if (Sound_effect == "ON")
	{
		thread clickThread(PlaySoundEffect, "click.wav");
		clickThread.detach();
	}
	if (Player1_name.length() > 14)
	{
		Player1_name = Player1_name.substr(0, 14);
	}
	if (Player2_name.length() > 14)
	{
		Player2_name = Player2_name.substr(0, 14);
	}
}

void DrawMenu()
{
	DrawMenuBackground();
	DrawNewGame(15);
	DrawLoadGame(15);
	DrawSettings(15);
	DrawHelp(15);
	DrawExit(15);
}

void DrawBack()
{
	BackgroundColor(6, 27, 10, 1, 15);
	SetColor(15, 10);
	DrawBox(5, 26, 10, 1);
	TextBox(6, 27, 10, "B:BACK");
}

void DrawHelpOption()
{
	DrawMenuBackground();
	SetColor(15, 0);
	DrawBox(25, 10, 70, 17);
	BackgroundColor(26, 11, 70, 17, 15);
	SetColor(15, 4);
	TextBox(26, 12, 70, "==================== HOW TO PLAY ====================");
	TextBox(26, 19, 70, "==================== ABOUT ====================");
	SetColor(15, 12);
	TextBox(26, 13, 70, "PRESS BUTTON A S D W TO MOVE.");
	TextBox(26, 14, 70, "PRESS ENTER FOR THE CHOICE.");
	TextBox(26, 15, 70, "WHO HAS 5 EXECUTIVE X OR O");
	TextBox(26, 16, 70, "IN LINE HORIZONTALLY OR VERTICALLY OR DIAGONALLY");
	TextBox(26, 17, 70, "WILL WIN THE GAME.");
	TextBox(26, 18, 70, "PRESS L TO SAVE GAME.");
	TextBox(26, 20, 70, "MEMBERS OF GROUP 10");
	TextBox(26, 21, 70, "MAI THUC HAI DANG - 24120276");
	TextBox(26, 22, 70, "DAO DAI GIA - 24120298");
	TextBox(26, 23, 70, "TRAN VO TAN DUNG - 24120291");
	TextBox(26, 24, 70, "NGUYEN VAN THANH TUNG - 24120241");
	TextBox(26, 25, 70, "THAI HOANG - 24120316");
	TextBox(26, 26, 70, "INSTRUCTOR: Dr.TRUONG TOAN THINH, Ph.D");
	DrawBack();
}

void DrawNewGameOption()
{
	DrawMenuBackground();
	Draw1Player(15);
	Draw2Player(15);
	DrawBack();
}

void DrawLoadGameOption()
{
	int i, k = 0, p = 0;
	DrawMenuBackground();
	GotoXY(40, 14);
	cout << "File saved:";
	for (i = 0; FileName[i] != ""; i++)
	{
		GotoXY(40, 15 + i);
		cout << i + 1 << ". " << FileName[i];
		if (i == 9) break;
	}
	GotoXY(40, 16 + i);
	cout << "or press ESC to back to menu";
	GotoXY(40, 15 + i);
	cout << "Enter the number of file you want to reload (maximum 15 characters): ";
	char ch;
	while (1)
	{
		ch = _getch();
		if (ch == 27)
		{
			if (Sound_effect == "ON")
			{
				thread clickThread(PlaySoundEffect, "click.wav");
				clickThread.detach();
			}
			return;
		}
		if (ch == 13)
		{
			if (Sound_effect == "ON")
			{
				thread clickThread(PlaySoundEffect, "click.wav");
				clickThread.detach();
			}
			if (k <= i) FileNameEnter = FileName[k - 1];
			return;
		}
		if (k >= 0 && k < 100 && ch >= '0' && ch <= '9')
		{
			if (p == 0 && ch == '0') continue;
			cout << ch;
			k = k * pow(10, p++) + ch - '0';
		}
	}
}

void DrawSaveGameOption()
{
	DrawMenuBackground();
	GotoXY(40, 14);
	cout << "Enter name of file you want to save (maximum 15 characters): ";
	char ch;
	while (FileNameEnter.length() < 15)
	{
		ch = _getch();
		if (ch == 13)
		{
			if (Sound_effect == "ON")
			{
				thread clickThread(PlaySoundEffect, "click.wav");
				clickThread.detach();
			}
			break;
		}
		if (ch == 27)
		{
			if (Sound_effect == "ON")
			{
				thread clickThread(PlaySoundEffect, "click.wav");
				clickThread.detach();
			}
			return;
		}
		if (ch == 8) continue;
		cout << ch;
		FileNameEnter += ch;
	}
}

void DrawSettingsOption()
{
	DrawMenuBackground();
	DrawMusic(14);
	DrawSoundEffect(15);
	DrawBack();
}

void DrawWin(int x, int y, int color)
{
	SetColor(color, color);
	int a[7][26] = { {0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,1,0,0,0,0,0,0,1,0,0},
					 {0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,1,0,0},
					 {0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1,0,0},
					 {0,0,1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0},
					 {0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,1,0,0,0,0,1,0,0,1,0,0},
					 {0,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,0},
					 {0,0,1,1,0,0,0,0,0,0,1,1,0,1,0,1,0,0,0,0,0,0,1,1,0,0} };
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			if (a[i][j])
			{
				GotoXY(x + j, y + i);
				cout << " ";
			}
		}
	}
}

void DrawDraw(int color)
{
	SetColor(color, color);
	int a[5][26] = { {0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0},
					 {0,0,0,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0},
					 {0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0},
					 {0,0,0,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,1,1,1,0,0,0,0,0},
					 {0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0} },
		b[5][26] = { {0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0},
					 {0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0},
					 {0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,0},
					 {0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,1,1,0,1,1,0,1,1,0},
					 {0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,0} };
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			if (a[i][j])
			{
				GotoXY(2 + j, 22 + i);
				cout << " ";
			}
			if (b[i][j])
			{
				GotoXY(92 + j, 22 + i);
				cout << " ";
			}
		}
	}
}

void DrawX()
{
	SetColor(15, 4);
	GotoXY(3, 6);  cout << " XXX               XXX  ";
	GotoXY(3, 7);  cout << "   XXX           XXX    ";
	GotoXY(3, 8);  cout << "     XXX       XXX      ";
	GotoXY(3, 9);  cout << "       XXX   XXX        ";
	GotoXY(3, 10); cout << "         XXXXX          ";
	GotoXY(3, 11); cout << "         XXXXX          ";
	GotoXY(3, 12); cout << "       XXX   XXX        ";
	GotoXY(3, 13); cout << "     XXX       XXX      ";
	GotoXY(3, 14); cout << "   XXX           XXX    ";
	GotoXY(3, 15); cout << " XXX               XXX  ";
}

void DrawO()
{
	SetColor(15, 3);
	GotoXY(93, 6);  cout << "        OOOOOOOO        ";
	GotoXY(93, 7);  cout << "     OOO        OOO     ";
	GotoXY(93, 8);  cout << "   OOO            OOO   ";
	GotoXY(93, 9);  cout << " OOO                OOO ";
	GotoXY(93, 10); cout << "OOO                  OOO";
	GotoXY(93, 11); cout << "OOO                  OOO";
	GotoXY(93, 12); cout << " OOO                OOO ";
	GotoXY(93, 13); cout << "   OOO            OOO   ";
	GotoXY(93, 14); cout << "     OOO        OOO     ";
	GotoXY(93, 15); cout << "        OOOOOOOO        ";
}

void DeleteX()
{
	SetColor(15, 4);
	GotoXY(3, 6);  cout << "                        ";
	GotoXY(3, 7);  cout << "                        ";
	GotoXY(3, 8);  cout << "                        ";
	GotoXY(3, 9);  cout << "                        ";
	GotoXY(3, 10); cout << "                        ";
	GotoXY(3, 11); cout << "                        ";
	GotoXY(3, 12); cout << "                        ";
	GotoXY(3, 13); cout << "                        ";
	GotoXY(3, 14); cout << "                        ";
	GotoXY(3, 15); cout << "                        ";
}

void DeleteO()
{
	SetColor(15, 3);
	GotoXY(93, 6);  cout << "                        ";
	GotoXY(93, 7);  cout << "                        ";
	GotoXY(93, 8);  cout << "                        ";
	GotoXY(93, 9);  cout << "                        ";
	GotoXY(93, 10); cout << "                        ";
	GotoXY(93, 11); cout << "                        ";
	GotoXY(93, 12); cout << "                        ";
	GotoXY(93, 13); cout << "                        ";
	GotoXY(93, 14); cout << "                        ";
	GotoXY(93, 15); cout << "                        ";
}

void WinEffect(int player_win)
{
	int x, y = 21;
	if (player_win == -1) x = 2;
	else x = 92;
	DrawWin(x, y, 2);
	Sleep(500);
	DrawWin(x, y, 1);
	Sleep(500);
	DrawWin(x, y, 4);
	Sleep(500);
	DrawWin(x, y, 2);
	Sleep(500);
	DrawWin(x, y, 1);
	Sleep(500);
	DrawWin(x, y, 4);
	Sleep(500);
	DrawWin(x, y, 2);
	Sleep(500);
	DrawWin(x, y, 1);
	Sleep(500);
	DrawWin(x, y, 4);
	Sleep(500);
}

void DrawEffect()
{
	DrawDraw(2);
	Sleep(500);
	DrawDraw(1);
	Sleep(500);
	DrawDraw(4);
	Sleep(500);
	DrawDraw(2);
	Sleep(500);
	DrawDraw(1);
	Sleep(500);
	DrawDraw(4);
	Sleep(500);
	DrawDraw(2);
	Sleep(500);
	DrawDraw(1);
	Sleep(500);
	DrawDraw(4);
	Sleep(500);
}

void AskContinue()
{
	SetColor(15, 10);
	TextBox(32, 0, 57, "Press \"Y\" to continue or \"N\" to stop the current game");
}

void DrawPvP()
{
	BackgroundColor(0, 0, 120, 30, 15);
	SetColor(15, 4);
	DrawBox(1, 1, 26, 15);
	DrawBox(1, 19, 26, 9);
	GotoXY(3, 2);
	cout << "Player 1: " << Player1_name;
	GotoXY(11, 3);
	cout << "Step: " << Step1;
	GotoXY(11, 4);
	cout << "Score: " << Score1;
	SetColor(15, 3);
	DrawBox(91, 1, 26, 15);
	DrawBox(91, 19, 26, 9);
	GotoXY(93, 2);
	cout << "Player 2: " << Player2_name;
	GotoXY(101, 3);
	cout << "Step: " << Step2;
	GotoXY(101, 4);
	cout << "Score: " << Score2;
	SetColor(15, 0);
	DrawBoard(LEFT, TOP, BOARD_SIZE, BOARD_SIZE);
	SetColor(15, 10);
	TextBox(1, 18, 28, "Press \"ESC\" to back to Menu");
	TextBox(91, 18, 28, "Press \"L\" to save game");
}

void DrawPvC()
{
	BackgroundColor(0, 0, 120, 30, 15);
	SetColor(15, 4);
	DrawBox(1, 1, 26, 15);
	DrawBox(1, 19, 26, 9);
	GotoXY(3, 2);
	cout << " Player: " << Player1_name;
	GotoXY(11, 3);
	cout << "Step: " << Step1;
	GotoXY(11, 4);
	cout << "Score: " << Score1;
	SetColor(15, 3);
	DrawBox(91, 1, 26, 15);
	DrawBox(91, 19, 26, 9);
	TextBox(92, 2, 26, "Computer");
	GotoXY(101, 3);
	cout << "Step: " << Step2;
	GotoXY(101, 4);
	cout << "Score: " << Score2;
	SetColor(15, 0);
	DrawBoard(LEFT, TOP, BOARD_SIZE, BOARD_SIZE);
	SetColor(15, 10);
	TextBox(1, 18, 28, "Press \"ESC\" to back to Menu");
	TextBox(91, 18, 28, "Press \"L\" to save game");
}