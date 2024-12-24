#include <iostream>
#include <conio.h>
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

//Di chuyển khi ở màn hình menu
void MenuUp();
void MenuDown();
//Di chuyển khi ở màn hình game
void GoUp();
void GoDown();
void GoRight();
void GoLeft();
void ResetBoard(); //Làm mới bàn cờ sau khi chơi xong
void PlaySoundEffect(const string& filename);
void PlayPvP();
void PlayPvC();

//View.cpp
void DeleteConsole();
void GotoXY(int x, int y);
void SetColor(int backgound_color, int text_color);
void BackgroundColor(int x, int y, int width, int height, int color);
void DrawLoadGameOption();
void DrawSaveGameOption();
void DrawX();
void DrawO();
void DeleteX();
void DeleteO();
void WinEffect(int player_win);
void DrawEffect();
void AskContinue();
void DrawPvP();
void DrawPvC();

//Model.cpp
int PlayerWin(int turn);
void SaveData(string filename);
void LoadData(string filename);
void AddFileNameToFile(string filename);


void MenuUp()
{
	if (_OPTION > 1)
	{
		_Y += 3;
		_OPTION--;
		if (Sound_effect == "ON")
		{
			thread clickThread(PlaySoundEffect, "click.wav");
			clickThread.detach();
		}
	}
}

void MenuDown()
{
	if (_OPTION < _SCREEN_OPTION[_SCREEN])
	{
		_Y -= 3;
		_OPTION++;
		if (Sound_effect == "ON")
		{
			thread clickThread(PlaySoundEffect, "click.wav");
			clickThread.detach();
		}
	}
}

void GoUp()
{
	if (_BOARD_X > 0)
	{
		if (_A[_BOARD_X][_BOARD_Y] == 0) BackgroundColor(_X - 1, _Y, 3, 1, 15);
		_Y -= 2;
		_BOARD_X--;
		if (_A[_BOARD_X][_BOARD_Y] == 0) BackgroundColor(_X - 1, _Y, 3, 1, 14);
		if (Sound_effect == "ON")
		{
			thread clickThread(PlaySoundEffect, "click.wav");
			clickThread.detach();
		}
	}
}

void GoDown()
{
	if (_BOARD_X < BOARD_SIZE - 1)
	{
		if (_A[_BOARD_X][_BOARD_Y] == 0) BackgroundColor(_X - 1, _Y, 3, 1, 15);
		_Y += 2;
		_BOARD_X++;
		if (_A[_BOARD_X][_BOARD_Y] == 0) BackgroundColor(_X - 1, _Y, 3, 1, 14);
		if (Sound_effect == "ON")
		{
			thread clickThread(PlaySoundEffect, "click.wav");
			clickThread.detach();
		}
	}
}

void GoRight()
{
	if (_BOARD_Y < BOARD_SIZE - 1)
	{
		if (_A[_BOARD_X][_BOARD_Y] == 0) BackgroundColor(_X - 1, _Y, 3, 1, 15);
		_X += 4;
		_BOARD_Y++;
		if (_A[_BOARD_X][_BOARD_Y] == 0) BackgroundColor(_X - 1, _Y, 3, 1, 14);
		if (Sound_effect == "ON")
		{
			thread clickThread(PlaySoundEffect, "click.wav");
			clickThread.detach();
		}
	}
}

void GoLeft()
{
	if (_BOARD_Y > 0)
	{
		if (_A[_BOARD_X][_BOARD_Y] == 0) BackgroundColor(_X - 1, _Y, 3, 1, 15);
		_X -= 4;
		_BOARD_Y--;
		if (_A[_BOARD_X][_BOARD_Y] == 0) BackgroundColor(_X - 1, _Y, 3, 1, 14);
		if (Sound_effect == "ON")
		{
			thread clickThread(PlaySoundEffect, "click.wav");
			clickThread.detach();
		}
	}
}

void ResetBoard()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			_A[i][j] = 0;
		}
	}
}

void PlaySoundEffect(const string& filename) {
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(filename))
	{
		cerr << "Failed to load sound effect file: " << filename << endl;
		return;
	}
	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.play();
	while (sound.getStatus() == sf::Sound::Playing)
	{
		sf::sleep(sf::milliseconds(10));
	}
}

void PlayPvP()
{
	
	if (isLoadGame)
	{
		DrawPvP();
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				GotoXY(LEFT + 2 + 4 * j, TOP + 1 + 2 * i);
				if (_A[i][j] == -1)
				{
					SetColor(15, 4);
					cout << "X";
				}
				if (_A[i][j] == 1)
				{
					SetColor(15, 3);
					cout << "O";
				}
			}
		}
	}
	while (1)
	{
		int win_flag = 0, esc_flag = 0;
		if (!isLoadGame) DrawPvP();
		if (_A[_BOARD_X][_BOARD_Y] == 0) BackgroundColor(_X - 1, _Y, 3, 1, 14);
		while (1)
		{
			if (_TURN == -1) DrawX();
			else DrawO();
			_COMMAND = toupper(_getch());
			switch (_COMMAND)
			{
			case 'A':
				GoLeft();
				break;
			case 'S':
				GoDown();
				break;
			case 'D':
				GoRight();
				break;
			case 'W':
				GoUp();
				break;
			case 13:
				if (_A[_BOARD_X][_BOARD_Y] == 0)
				{
					_A[_BOARD_X][_BOARD_Y] = _TURN;
					GotoXY(_X, _Y);
					if (_TURN == -1)
					{
						SetColor(15, 4);
						cout << "X";
						GotoXY(17, 3);
						cout << ++Step1;
						BackgroundColor(_X - 1, _Y, 1, 1, 15);
						BackgroundColor(_X + 1, _Y, 1, 1, 15);
						if (Sound_effect == "ON")
						{
							thread clickThread(PlaySoundEffect, "click.wav");
							clickThread.detach();
						}
						if (PlayerWin(-1))
						{
							win_flag = -1;
							if (Sound_effect == "ON")
							{
								thread clickThread(PlaySoundEffect, "win.wav");
								clickThread.detach();
							}
							WinEffect(-1);
							break;
						}
						DeleteX();
						_TURN = 1;
					}
					else
					{
						SetColor(15, 3);
						cout << "O";
						GotoXY(107, 3);
						cout << ++Step2;
						BackgroundColor(_X - 1, _Y, 1, 1, 15);
						BackgroundColor(_X + 1, _Y, 1, 1, 15);
						if (Sound_effect == "ON")
						{
							thread clickThread(PlaySoundEffect, "click.wav");
							clickThread.detach();
						}
						if (PlayerWin(1))
						{
							win_flag = 1;
							if (Sound_effect == "ON")
							{
								thread clickThread(PlaySoundEffect, "win.wav");
								clickThread.detach();
							}
							WinEffect(1);
							break;
						}
						DeleteO();
						_TURN = -1;
					}
				}
				break;
			case 27:
				if (Sound_effect == "ON")
				{
					thread clickThread(PlaySoundEffect, "click.wav");
					clickThread.detach();
				}
				esc_flag = 1;
				break;
			case 'L':
				if (Sound_effect == "ON")
				{
					thread clickThread(PlaySoundEffect, "click.wav");
					clickThread.detach();
				}
				int i;
				for (i = 0; FileName[i] != ""; i++)
					if (i == 9)
					{
						esc_flag = 1;
						break;
					}
				DrawSaveGameOption();
				FileName[i] = FileNameEnter;
				SaveData(FileName[i]);
				AddFileNameToFile(FileName[i]);
				FileNameEnter = "";
				esc_flag = 1;
				break;
			}
			if (win_flag == -1)
			{
				Score1++;
				AskContinue();
				while (1)
				{
					_COMMAND = toupper(_getch());
					if (_COMMAND == 'Y')
					{
						if (Sound_effect == "ON")
						{
							thread clickThread(PlaySoundEffect, "click.wav");
							clickThread.detach();
						}
						break;
					}
					else if (_COMMAND == 'N' || _COMMAND == 27)
					{
						if (Sound_effect == "ON")
						{
							thread clickThread(PlaySoundEffect, "click.wav");
							clickThread.detach();
						}
						esc_flag = 1;
						break;
					}
				}
				break;
			}
			else if (win_flag == 1)
			{
				Score2++;
				AskContinue();
				while (1)
				{
					_COMMAND = toupper(_getch());
					if (_COMMAND == 'Y')
					{
						if (Sound_effect == "ON")
						{
							thread clickThread(PlaySoundEffect, "click.wav");
							clickThread.detach();
						}
						break;
					}
					else if (_COMMAND == 'N' || _COMMAND == 27)
					{
						if (Sound_effect == "ON")
						{
							thread clickThread(PlaySoundEffect, "click.wav");
							clickThread.detach();
						}
						esc_flag = 1;
						break;
					}
				}
				break;
			}
			else if (Step2 == 98)
			{
				if (Sound_effect == "ON")
				{
					thread clickThread(PlaySoundEffect, "draw.wav");
					clickThread.detach();
				}
				DrawEffect();
				AskContinue();
				while (1)
				{
					_COMMAND = toupper(_getch());
					if (_COMMAND == 'Y') break;
					else if (_COMMAND == 'N' || _COMMAND == 27) esc_flag = 1;
				}
				break;
			}
			if (esc_flag) break;
		}
		Step1 = Step2 = 0;
		_BOARD_X = _BOARD_Y = 0;
		_TURN = -1;
		ResetBoard();
		_X = LEFT + 2;
		_Y = TOP + 1;
		isLoadGame = 0;
		if (esc_flag) break;
	}
	Score1 = Score2 = 0;
	Player1_name = Player2_name = "";
}

void PlayPvC()
{
	if (isLoadGame)
	{
		DrawPvC();
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				GotoXY(LEFT + 2 + 4 * j, TOP + 1 + 2 * i);
				if (_A[i][j] == -1)
				{
					SetColor(15, 4);
					cout << "X";
				}
				if (_A[i][j] == 1)
				{
					SetColor(15, 3);
					cout << "O";
				}
			}
		}
	}
	while (1)
	{
		int win_flag = 0, esc_flag = 0, computer_flag = 0;
		if (!isLoadGame) DrawPvC();
		DrawX();
		if (_A[_BOARD_X][_BOARD_Y] == 0) BackgroundColor(_X - 1, _Y, 3, 1, 14);
		while (1)
		{
			if (_TURN == -1)
			{
				_COMMAND = toupper(_getch());
				switch (_COMMAND)
				{
				case 'A':
					GoLeft();
					break;
				case 'S':
					GoDown();
					break;
				case 'D':
					GoRight();
					break;
				case 'W':
					GoUp();
					break;
				case 13:
					if (_A[_BOARD_X][_BOARD_Y] == 0)
					{
						_A[_BOARD_X][_BOARD_Y] = -1;
						GotoXY(_X, _Y);
						SetColor(15, 4);
						cout << "X";
						GotoXY(17, 3);
						cout << ++Step1;
						BackgroundColor(_X - 1, _Y, 1, 1, 15);
						BackgroundColor(_X + 1, _Y, 1, 1, 15);
						if (Sound_effect == "ON")
						{
							thread clickThread(PlaySoundEffect, "click.wav");
							clickThread.detach();
						}
						if (PlayerWin(-1))
						{
							win_flag = -1;
							if (Sound_effect == "ON")
							{
								thread clickThread(PlaySoundEffect, "win.wav");
								clickThread.detach();
							}
							WinEffect(-1);
							break;
						}
						_TURN = 1;
					}
					break;
				case 27:
					if (Sound_effect == "ON")
					{
						thread clickThread(PlaySoundEffect, "click.wav");
						clickThread.detach();
					}
					esc_flag = 1;
					break;
				case 'L':
					if (Sound_effect == "ON")
					{
						thread clickThread(PlaySoundEffect, "click.wav");
						clickThread.detach();
					}
					int i;
					for (i = 0; FileName[i] != ""; i++)
						if (i == 9)
						{
							esc_flag = 1;
							break;
						}
					DrawSaveGameOption();
					FileName[i] = FileNameEnter;
					SaveData(FileName[i]);
					AddFileNameToFile(FileName[i]);
					FileNameEnter = "";
					esc_flag = 1;
					break;
				}
			}
			else
			{
				computer_flag = 0;
				for (int i = 0; i < BOARD_SIZE; i++)
				{
					for (int j = 0; j < BOARD_SIZE; j++)
					{
						if (_A[i][j] == 0)
						{
							_A[i][j] = 1;
							_BOARD_X = i;
							_BOARD_Y = j;
							_X = LEFT + 2 + 4 * j;
							_Y = TOP + 1 + 2 * i;
							GotoXY(_X, _Y);
							SetColor(15, 3);
							cout << "O";
							_TURN = -1;
							GotoXY(107, 3);
							cout << ++Step2;
							if (PlayerWin(1))
							{
								win_flag = 1;
								DeleteX();
								DrawO();
								if (Sound_effect == "ON")
								{
									thread clickThread(PlaySoundEffect, "win.wav");
									clickThread.detach();
								}
								WinEffect(1);
							}
							computer_flag = 1;
							break;
						}
					}
					if (computer_flag) break;
				}
			}
			if (win_flag == -1)
			{
				Score1++;
				AskContinue();
				while (1)
				{
					_COMMAND = toupper(_getch());
					if (_COMMAND == 'Y')
					{
						if (Sound_effect == "ON")
						{
							thread clickThread(PlaySoundEffect, "click.wav");
							clickThread.detach();
						}
						break;
					}
					else if (_COMMAND == 'N' || _COMMAND == 27)
					{
						if (Sound_effect == "ON")
						{
							thread clickThread(PlaySoundEffect, "click.wav");
							clickThread.detach();
						}
						esc_flag = 1;
						break;
					}
				}
				break;
			}
			else if (win_flag == 1)
			{
				Score2++;
				AskContinue();
				while (1)
				{
					_COMMAND = toupper(_getch());
					if (_COMMAND == 'Y')
					{
						if (Sound_effect == "ON")
						{
							thread clickThread(PlaySoundEffect, "click.wav");
							clickThread.detach();
						}
						break;
					}
					else if (_COMMAND == 'N' || _COMMAND == 27)
					{
						if (Sound_effect == "ON")
						{
							thread clickThread(PlaySoundEffect, "click.wav");
							clickThread.detach();
						}
						esc_flag = 1;
						break;
					}
				}
				break;
			}
			else if (Step2 == 98)
			{
				if (Sound_effect == "ON")
				{
					thread clickThread(PlaySoundEffect, "draw.wav");
					clickThread.detach();
				}
				DrawEffect();
				AskContinue();
				while (1)
				{
					_COMMAND = toupper(_getch());
					if (_COMMAND == 'Y') break;
					else if (_COMMAND == 'N' || _COMMAND == 27) esc_flag = 1;
				}
				break;
			}
			if (esc_flag) break;
		}
		Step1 = Step2 = 0;
		_BOARD_X = _BOARD_Y = 0;
		_TURN = -1;
		ResetBoard();
		_X = LEFT + 2;
		_Y = TOP + 1;
		isLoadGame = 0;
		if (esc_flag) break;
	}
	Score1 = Score2 = 0;
	Player1_name = Player2_name = "";
}