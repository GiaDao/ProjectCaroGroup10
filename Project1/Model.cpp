#include <iostream>
#include <conio.h>
#include <fstream>
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
sf::Music music;

//Các trường hợp thắng game
int WinRow(int turn);
int WinColumn(int turn);
int WinLeftDiagonal1(int turn);
int WinLeftDiagonal2(int turn);
int WinRightDiagonal1(int turn);
int WinRightDiagonal2(int turn);
int TotalWin(int turn); //Kết hợp các trường hợp thắng game
int PlayerWin(int turn); //Cho biết người chơi turn thắng hay không
void SaveData(string filename);
void LoadData(string filename);
void SetFileName(); //Lấy tên các file đã lưu trước đó từ file Filename.txt
void AddFileNameToFile(string filename); //Thêm tên các file mới lưu xong vào file Filename.txt
void PlayMusic(const string& filename);
void PlaySoundEffect(const string& filename);
void Play();

//View.cpp
void DeleteConsole();
void GotoXY(int x, int y);
void SetColor(int backgound_color, int text_color);
void DrawNewGame(int background_color);
void DrawLoadGame(int background_color);
void DrawSettings(int background_color);
void DrawHelp(int background_color);
void DrawExit(int background_color);
void DrawMusic(int background_color);
void DrawSoundEffect(int background_color);
void Draw1Player(int background_color);
void Draw1PlayerOption();
void Draw2Player(int background_color);
void Draw2PlayerOption();
void DrawMenu();
void DrawHelpOption();
void DrawNewGameOption();
void DrawLoadGameOption();
void DrawSettingsOption();

//Control.cpp
void MenuUp();
void MenuDown();
void ResetBoard();
void PlaySoundEffect(const string& filename);
void PlayPvP();
void PlayPvC();


int WinRow(int turn)
{
	int count;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		count = 0;
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (_A[i][j] == turn)
			{
				count++;
				if (count == 5) return 1;
			}
			else count = 0;
		}
	}
	return 0;
}

int WinColumn(int turn)
{
	int count;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		count = 0;
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (_A[j][i] == turn)
			{
				count++;
				if (count == 5) return 1;
			}
			else count = 0;
		}
	}
	return 0;
}

int WinLeftDiagonal1(int turn)
{
	int count;
	for (int i = 0; i < BOARD_SIZE - 4; i++)
	{
		count = 0;
		for (int j = 0; j < BOARD_SIZE - i; j++)
		{
			if (_A[i + j][j] == turn)
			{
				count++;
				if (count == 5) return 1;
			}
			else count = 0;
		}
	}
	return 0;
}

int WinLeftDiagonal2(int turn)
{
	int count;
	for (int i = 1; i < BOARD_SIZE - 4; i++)
	{
		count = 0;
		for (int j = 0; j < BOARD_SIZE - i; j++)
		{
			if (_A[j][i + j] == turn)
			{
				count++;
				if (count == 5) return 1;
			}
			else count = 0;
		}
	}
	return 0;
}

int WinRightDiagonal1(int turn)
{
	int count;
	for (int i = 0; i < BOARD_SIZE - 4; i++)
	{
		count = 0;
		for (int j = 0; j < BOARD_SIZE - i; j++)
		{
			if (_A[i + j][BOARD_SIZE - 1 - j] == turn)
			{
				count++;
				if (count == 5) return 1;
			}
			else count = 0;
		}
	}
	return 0;
}

int WinRightDiagonal2(int turn)
{
	int count;
	for (int i = 1; i < BOARD_SIZE - 4; i++)
	{
		count = 0;
		for (int j = 0; j < BOARD_SIZE - i; j++)
		{
			if (_A[j][BOARD_SIZE - 1 - i - j] == turn)
			{
				count++;
				if (count == 5) return 1;
			}
			else count = 0;
		}
	}
	return 0;
}

int TotalWin(int turn)
{
	return WinRow(turn) + WinColumn(turn) + WinLeftDiagonal1(turn) + WinLeftDiagonal2(turn) + WinRightDiagonal1(turn) + WinRightDiagonal2(turn);
}

int PlayerWin(int turn)
{
	if (TotalWin(turn)) return 1;
	return 0;
}

void SaveData(string filename) {
	ofstream savefile;
	savefile.open(filename, ios::out);
	savefile << Player1_name << endl;
	savefile << Score1 << endl;
	savefile << Step1 << endl;
	savefile << Player2_name << endl;
	savefile << Score2 << endl;
	savefile << Step2 << endl;
	savefile << _X << endl;
	savefile << _Y << endl;
	savefile << _BOARD_X << endl;
	savefile << _BOARD_Y << endl;
	savefile << _TURN << endl;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			savefile << _A[i][j] << " ";
		}
		savefile << endl;
	}
	savefile.close();
}

void LoadData(string filename) {
	ifstream loadfile;
	loadfile.open(filename, ios::in);
	getline(loadfile, Player1_name);
	loadfile >> Score1 >> Step1;
	loadfile.ignore();
	getline(loadfile, Player2_name);
	loadfile >> Score2 >> Step2 >> _X >> _Y >> _BOARD_X >> _BOARD_Y >> _TURN;
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			loadfile >> _A[i][j];
	loadfile.close();
}

void SetFileName()
{
	FILE* f;
	errno_t err = fopen_s(&f, "Filename.txt", "r");
	int i = 0;
	char t[16];
	while (fgets(t, sizeof(t), f) != NULL)
	{
		if (strlen(t) > 0 && t[strlen(t) - 1] == '\n')
		{
			t[strlen(t) - 1] = '\0';
		}
		FileName[i++] = t;
	}
	fclose(f);
}

void AddFileNameToFile(string filename)
{
	FILE* f;
	errno_t err = fopen_s(&f, "Filename.txt", "a");
	fprintf(f, "%s\n", filename.c_str());
	fclose(f);
}

void PlayMusic(const string& filename) {
	if (!music.openFromFile(filename)) {
		cerr << "Failed to load music file: " << filename << std::endl;
		return;
	}
	music.setLoop(true);
	music.play();
	while (music.getStatus() == sf::Music::Playing) {
		sf::sleep(sf::seconds(1));
	}
}

void Play()
{
	thread backgroundMusic(PlayMusic, "background.wav");
	backgroundMusic.detach();
	ResetBoard();
	SetFileName();
	int esc_flag = 0, change = 0;
	while (1)
	{
		int newgame_flag = 0;
		DeleteConsole();
		DrawMenu();
		DrawNewGame(14);
		_OPTION = 1;
		_SCREEN = 0;
		while (1)
		{
			_COMMAND = toupper(_getch());
			switch (_COMMAND)
			{
			case 'W':
				MenuUp();
				break;
			case 'S':
				MenuDown();
				break;
			case 13:
				if (Sound_effect == "ON")
				{
					thread clickThread(PlaySoundEffect, "click.wav");
					clickThread.detach();
				}
				if (_SCREEN == 0 && _OPTION == 1)
				{
					DeleteConsole();
					DrawNewGameOption();
					Draw1Player(14);
					_SCREEN++;
				}
				else if (_SCREEN == 0 && _OPTION == 2)
				{
					DeleteConsole();
					DrawLoadGameOption();
					for (int i = 0; FileName[i] != ""; i++)
					{
						if (FileNameEnter == FileName[i])
						{
							LoadData(FileNameEnter);
							FileNameEnter = "";
							isLoadGame = 1;
							if (Player2_name == "") PlayPvC();
							else PlayPvP();
							break;
						}
						if (i == 9) break;
					}
					isLoadGame = 0;
					newgame_flag = 1;
					_OPTION = 1;
				}
				else if (_SCREEN == 0 && _OPTION == 3)
				{
					_SCREEN = 2;
					_OPTION = 1;
					DrawSettingsOption();
				}
				else if (_SCREEN == 0 && _OPTION == 4)
				{
					DeleteConsole();
					DrawHelpOption();
					_SCREEN = 3;
				}
				else if (_SCREEN == 0 && _OPTION == 5)
				{
					esc_flag = 1;
				}
				else if (_SCREEN == 1 && _OPTION == 1)
				{
					Draw1PlayerOption();
					_X = LEFT + 2;
					_Y = TOP + 1;
					PlayPvC();
					newgame_flag = 1;
				}
				else if (_SCREEN == 1 && _OPTION == 2)
				{
					Draw2PlayerOption();
					_X = LEFT + 2;
					_Y = TOP + 1;
					PlayPvP();
					newgame_flag = 1;
				}
				else if (_SCREEN == 2 && _OPTION == 1)
				{
					SetColor(14, 0);
					GotoXY(62, 15);
					if (Music == "ON")
					{
						Music = "OFF";
						cout << "OFF";
						change = 1;
					}
					else
					{
						Music = "ON";
						cout << "ON ";
						change = 1;
					}
				}
				else if (_SCREEN == 2 && _OPTION == 2)
				{
					SetColor(14, 0);
					GotoXY(66, 18);
					if (Sound_effect == "ON")
					{
						Sound_effect = "OFF";
						cout << "OFF";
					}
					else
					{
						Sound_effect = "ON";
						cout << "ON ";
					}
				}
				break;
			case 'B':
				if (_SCREEN != 0)
				{
					if (Sound_effect == "ON")
					{
						thread clickThread(PlaySoundEffect, "click.wav");
						clickThread.detach();
					}
					_SCREEN = 0;
					_OPTION = 1;
					DeleteConsole();
					DrawMenu();
					DrawNewGame(14);
					DrawLoadGame(15);
				}
				break;
			}
			if (_SCREEN == 0 && !newgame_flag)
			{
				switch (_OPTION)
				{
				case 1:
					DrawNewGame(14);
					DrawLoadGame(15);
					break;
				case 2:
					DrawNewGame(15);
					DrawLoadGame(14);
					DrawSettings(15);
					break;
				case 3:
					DrawLoadGame(15);
					DrawSettings(14);
					DrawHelp(15);
					break;
				case 4:
					DrawSettings(15);
					DrawHelp(14);
					DrawExit(15);
					break;
				case 5:
					DrawHelp(15);
					DrawExit(14);
					break;
				}
			}
			if (_SCREEN == 1 && _OPTION == 1 && !newgame_flag)
			{
				Draw1Player(14);
				Draw2Player(15);
			}
			else if (_SCREEN == 1 && _OPTION == 2 && !newgame_flag)
			{
				Draw1Player(15);
				Draw2Player(14);
			}
			if (_SCREEN == 2 && _OPTION == 1)
			{
				DrawMusic(14);
				DrawSoundEffect(15);
			}
			else if (_SCREEN == 2 && _OPTION == 2)
			{
				DrawMusic(15);
				DrawSoundEffect(14);
			}
			if (Music == "OFF" && change)
			{
				music.stop();
				change = 0;
			}
			else if (Music == "ON" && change)
			{
				music.play();
				change = 0;
			}
			if (esc_flag || newgame_flag) break;
		}
		if (esc_flag)
		{
			GotoXY(0, 29);
			cout << endl;
			break;
		}
	}
}