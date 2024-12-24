#include <iostream>
using namespace std;

#define BOARD_SIZE 14 //Kích thước ma trận bàn cờ
#define LEFT 32 //Tọa độ trái màn hình bàn cờ
#define TOP 1 //Tọa độ trên màn hình bàn cờ

int _A[BOARD_SIZE][BOARD_SIZE]; //Ma trận bàn cờ
int _BOARD_X = 0, _BOARD_Y = 0; //Biến nhận giá trị là tọa độ của bàn cờ
int _TURN = -1; //-1 là lượt người thứ nhất và 1 là lượt người thứ hai
int _COMMAND; //Biến nhận giá trị phím người dùng nhập
int _X, _Y; //Tọa độ hiện hành trên màn hình bàn cờ
int _SCREEN; //Biến hiển thị đang ở màn hình menu nào
int _SCREEN_OPTION[4] = { 5,2,2,0 }; //Số lựa chọn trong mỗi màn hình menu
int _OPTION; //Thứ tự hộp menu
int Score1 = 0;
int Score2 = 0;
int Step1 = 0;
int Step2 = 0;
int isLoadGame = 0; //Biến nhận giá trị có tải game đã lưu không
string Player1_name = "";
string Player2_name = "";
string FileName[10]; //Tên các game đã lưu
string FileNameEnter = ""; //Biến nhận tên game mà người dùng nhập
string Music = "ON";
string Sound_effect = "ON";

//view.cpp
void FixConsoleWindow();

//model.cpp
void Play();

void main()
{
	FixConsoleWindow();
	Play();
}