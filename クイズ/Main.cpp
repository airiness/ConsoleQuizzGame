#include<Windows.h>
#include<iostream>
#include<string>
#include<vector>
using namespace std;

typedef struct
{
	int x;
	int y;
	int wide;
} WINDOWSIZE;

typedef struct
{
	int x;
	int y;
} BEGINPOSITION;

typedef struct
{
	string WindowFrameStyle;
	short WindowColor;
} WINDOWSTYLE;

typedef struct
{
	string Question;
	vector<string> Anwsers;
	int RightAnswer;
} QUIZZ;

int makeWindow(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, WINDOWSTYLE windowStyle);

int main()
{
	//--make the welcome window
	auto hWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	BEGINPOSITION welcomWindowBeginPosition = { 0,0 };
	WINDOWSIZE welcomeWindowSize = { 30,20,1 };
	WINDOWSTYLE welcomeWindowStyle = { "Åô",0xB };
	makeWindow(hWindow, welcomWindowBeginPosition, welcomeWindowSize, welcomeWindowStyle);
	cout << "what fuck with your head" << endl;
	//----------------------

	vector<QUIZZ> QuizzesFromFile;



	return 0;
}

int makeWindow(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, WINDOWSTYLE windowStyle)
{
	SetConsoleTextAttribute(hwindow, windowStyle.WindowColor);
	COORD cursorPosition;
	cursorPosition.X = bposition.x;
	cursorPosition.Y = bposition.y;
	string upLine = "";
	string leftAndRightLineWide = "";
	for (size_t uplongi = 0; uplongi < wsize.x; uplongi++)
	{
		upLine += windowStyle.WindowFrameStyle;
	}
	for (size_t uplongsize = 0; uplongsize < wsize.wide; uplongsize++)
	{
		leftAndRightLineWide += windowStyle.WindowFrameStyle;
		SetConsoleCursorPosition(hwindow, cursorPosition);
		cout << upLine;
		cursorPosition.Y++;
	}
	if (wsize.y > wsize.wide * 2)
	{
		for (size_t lArlongi = 0; lArlongi < (wsize.y - wsize.wide * 2); lArlongi++)
		{
			SetConsoleCursorPosition(hwindow, cursorPosition);
			cout << leftAndRightLineWide;
			cursorPosition.X += windowStyle.WindowFrameStyle.length() * (wsize.x - wsize.wide);
			SetConsoleCursorPosition(hwindow, cursorPosition);
			cout << leftAndRightLineWide;
			cursorPosition.X = bposition.x;
			cursorPosition.Y++;
		}
	}
	SetConsoleCursorPosition(hwindow, cursorPosition);
	for (size_t uplongsize = 0; uplongsize < wsize.wide; uplongsize++)
	{
		SetConsoleCursorPosition(hwindow, cursorPosition);
		cout << upLine;
		cursorPosition.Y++;
	}
	cursorPosition.X = bposition.x + windowStyle.WindowFrameStyle.length() * wsize.wide;
	cursorPosition.Y = bposition.y + wsize.wide;
	SetConsoleCursorPosition(hwindow, cursorPosition);
	SetConsoleTextAttribute(hwindow, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

	return 0;
}