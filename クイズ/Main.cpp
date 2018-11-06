#include<Windows.h>
#include<iostream>
#include<string>
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

int makeWindow(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, string windowStyle);

int main()
{
	HANDLE hWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	BEGINPOSITION welcomWindowBeginPosition = { 0,0 };
	WINDOWSIZE welcomeWindow = { 30,20,1 };
	string windowStyle = "Å°Å°";

	makeWindow(hWindow, welcomWindowBeginPosition, welcomeWindow, windowStyle);

	return 0;
}

int makeWindow(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, string windowStyle)
{
	int styleWide = windowStyle.length();
	COORD cursorPosition;
	cursorPosition.X = bposition.x;
	cursorPosition.Y = bposition.y;
	string upLine = "";
	string leftAndRightLineWide = "";
	for (size_t uplongi = 0; uplongi < wsize.x; uplongi++)
	{
		upLine += windowStyle;
	}
	for (size_t uplongsize = 0; uplongsize < wsize.wide; uplongsize++)
	{
		leftAndRightLineWide += windowStyle;
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
			cursorPosition.X += styleWide*(wsize.x - wsize.wide);
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



	return 0;
}