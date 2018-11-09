#include<Windows.h>
#include<conio.h>
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

const auto quizznum = 10;
const auto answernum = 3;

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
int showQuestion(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, WINDOWSTYLE windowStyle, string question);
void showList(HANDLE hwindow, vector<string> list, int index, BEGINPOSITION bposition, WINDOWSIZE wsize);
int getinput(int * index, int indexsize);


int main()
{
	system("mode con cols=143 lines=50  ");
	system("CLS");
	//--read quizz
	vector<QUIZZ> vQuizzs(quizznum);

	fstream fQuizz("quizz.txt");
	if (!fQuizz.is_open())
	{
		cout << "oh holy smokes!";
		getchar();
		return 0;
	}

	for (auto iter = vQuizzs.begin(); iter !=vQuizzs.end(); iter++)
	{
		if (fQuizz.eof())
		{
			break;
		}
		fQuizz >> iter->Question;
		for (auto j=0;j<3;j++)
		{
			string tmp;
			fQuizz >> tmp;
			iter->Anwsers.push_back(tmp);
		}
		fQuizz >> iter->RightAnswer;
	}
	fQuizz.close();
	//---

	//--make the welcome window
	auto hWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cc_info;
	cc_info.bVisible = 0;
	cc_info.dwSize = 1;
	SetConsoleCursorInfo(hWindow, &cc_info);

	COORD MainCursorPosition = { 0,0 };

	BEGINPOSITION MainWindowBeginPosition = { 10,5 };
	WINDOWSIZE MainWindowSize = { 60,40,1 };
	WINDOWSTYLE MainWindowStyle = { "■",0xC };
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

	BEGINPOSITION WelcomWindowBeginPosition = { 36,15 };
	WINDOWSIZE WelcomeWindowSize = { 30,20,1 };
	WINDOWSTYLE WelcomeWindowStyle = { "■",0xB };
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
	fWelcomeText.close();
	getchar();
	system("cls");

	//--begin show the quizzs
	auto index = 0;
	for (auto iter = vQuizzs.begin(); iter != vQuizzs.end(); iter++)
	{
		makeWindow(hWindow, MainWindowBeginPosition, MainWindowSize, MainWindowStyle);
		//show question
		showQuestion(hWindow, MainWindowBeginPosition, MainWindowSize, MainWindowStyle, iter->Question);
		//show answer
		while (true)
		{
			showList(hWindow, iter->Anwsers, index, MainWindowBeginPosition, MainWindowSize);
			getinput(&index, iter->Anwsers.size());

		}
	}




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

int showQuestion(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, WINDOWSTYLE windowStyle,string question)
{
	COORD cursorPosition;
	cursorPosition.X = bposition.x + wsize.wide * windowStyle.WindowFrameStyle.length();
	cursorPosition.Y = bposition.y + wsize.wide;
	SetConsoleCursorPosition(hwindow, cursorPosition);
	SetConsoleTextAttribute(hwindow, 0xB);
	if (question.length() < wsize.x - 2 * wsize.wide * windowStyle.WindowFrameStyle.length())
	{
		cout << question;
		cursorPosition.X = bposition.x + wsize.wide * windowStyle.WindowFrameStyle.length();
		cursorPosition.Y++;
		SetConsoleCursorPosition(hwindow, cursorPosition);
	}
	else 
	{
		vector<string> divideQuestion;
		int snum = question.length() / (wsize.x - 2 * wsize.wide * windowStyle.WindowFrameStyle.length());
		int lostnum = question.length() % (wsize.x - 2 * wsize.wide * windowStyle.WindowFrameStyle.length());
		for (size_t i = 0; i < snum; i++)
		{
			string tmp(question.substr(i * (wsize.x - 2 * wsize.wide * windowStyle.WindowFrameStyle.length(), (i + 1)*(wsize.x - 2 * wsize.wide * windowStyle.WindowFrameStyle.length()))));
			//divideQuestion.push_back(tmp);
			cout << tmp;
			cursorPosition.X = bposition.x + wsize.wide * windowStyle.WindowFrameStyle.length();
			cursorPosition.Y++;
			SetConsoleCursorPosition(hwindow, cursorPosition);
		}
		if (lostnum!=0)
		{
			string tmp(question.substr((snum * (wsize.x - 2 * wsize.wide * windowStyle.WindowFrameStyle.length())) - 1, lostnum));
			cout << tmp;
			cursorPosition.X = bposition.x + wsize.wide * windowStyle.WindowFrameStyle.length();
			cursorPosition.Y++;
			SetConsoleCursorPosition(hwindow, cursorPosition);
		}

	}

	return 0;
}

void showList(HANDLE hwindow, vector<string> list, int index, BEGINPOSITION bposition, WINDOWSIZE wsize)
{
	COORD cursorPosition;
	SetConsoleTextAttribute(hwindow, FOREGROUND_GREEN | 0x8);
	cursorPosition.X = bposition.x + wsize.x /3;
	cursorPosition.Y = bposition.y + 3;
	SetConsoleCursorPosition(hwindow, cursorPosition);
	for (auto i = 0; i<list.size(); i++)
	{
		if (i == index)//もし選んだら、赤になる
		{
			SetConsoleTextAttribute(hwindow, FOREGROUND_RED | 0x8);
			cursorPosition.X = bposition.x + wsize.x / 3;
			cursorPosition.Y ++;
			SetConsoleCursorPosition(hwindow, cursorPosition);
			cout << list[i];
		}
		else//選んだないもの白い色
		{
			SetConsoleTextAttribute(hwindow, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cursorPosition.X = bposition.x + wsize.x / 3;
			cursorPosition.Y++;
			SetConsoleCursorPosition(hwindow, cursorPosition);
			cout << list[i];
		}
	}
}

int getinput(int * index,int indexsize)
{
	int ch;
	ch = _getch();
	switch (ch)
	{
	case UP:
		if (*index > 0)
		{
			*index -= 1;
		}
		break;
	case DOWN:
		if (*index < indexsize - 1)
		{
			*index += 1;
		}
		break;
	case ESC:
		return ESC;
		break;
	case ENTER:
		return ENTER;
		break;
	default:
		break;
	}
	return 0;
}
