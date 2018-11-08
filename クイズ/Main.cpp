#include<Windows.h>
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

enum INPUTCOMMAND
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	ENTER = 13,
	ESC = 27,
};

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
	bool RightOrWrong;
} QUIZZ;



int makeWindow(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, WINDOWSTYLE windowStyle);

int main()
{
	//--read quizz
	vector<QUIZZ> vQuizzs;

	fstream fQuizz("quizz.txt");
	if (!fQuizz.is_open())
	{
		cout << "oh holy smokes!";
		getchar();
		return 0;
	}

	while (!fQuizz.eof())
	{

	}






	//--make the welcome window
	auto hWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cc_info;
	cc_info.bVisible = 0;
	cc_info.dwSize = 1;
	SetConsoleCursorInfo(hWindow, &cc_info);

	COORD MainCursorPosition = { 0,0 };

	BEGINPOSITION MainWindowBeginPosition = { 10,5 };
	WINDOWSIZE MainWindowSize = { 60,40,1 };
	WINDOWSTYLE MainWindowStyle = { "¡",0xC };
	makeWindow(hWindow, MainWindowBeginPosition, MainWindowSize, MainWindowStyle);
	//----------------------

	//--fill the text in welcome window
	fstream fWelcomeText("welcomtext.txt");
	if (!fWelcomeText.is_open())
	{
		cout << "oh holy smokes!";
		getchar();
		return 0;
	}

	BEGINPOSITION WelcomWindowBeginPosition = { 20,10 };
	WINDOWSIZE WelcomeWindowSize = { 30,20,1 };
	WINDOWSTYLE WelcomeWindowStyle = { "¡",0xB };
	makeWindow(hWindow, WelcomWindowBeginPosition, WelcomeWindowSize, WelcomeWindowStyle);
	MainCursorPosition.X = WelcomWindowBeginPosition.x + WelcomeWindowSize.wide;
	MainCursorPosition.Y = WelcomWindowBeginPosition.y + WelcomeWindowSize.wide;


	while (!fWelcomeText.eof())
	{
		string tempstring;
		fWelcomeText >> tempstring;
		cout << tempstring;
		MainCursorPosition.X = WelcomWindowBeginPosition.x + WelcomeWindowSize.wide * WelcomeWindowStyle.WindowFrameStyle.length();
		MainCursorPosition.Y++;
		SetConsoleCursorPosition(hWindow, MainCursorPosition);
	}

	SetConsoleTextAttribute(hWindow, FOREGROUND_RED);
	cout << "Press Any Button Go!";
	getchar();
	system("cls");
	makeWindow(hWindow, MainWindowBeginPosition, MainWindowSize, MainWindowStyle);




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

