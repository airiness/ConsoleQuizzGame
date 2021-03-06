#include<Windows.h>
#include<conio.h>
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

const auto quizznum = 10;//クイズ数量
const auto answernum = 3;//選択の数量

enum INPUTCOMMAND
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,
	ENTER = 13,
	ESC = 27,
};

//windowのサイズ
typedef struct
{
	int x;
	int y;
	int wide;
} WINDOWSIZE;

//Windowsのposition
typedef struct
{
	int x;
	int y;
} BEGINPOSITION;

//Windowsの形
typedef struct
{
	string WindowFrameStyle;
	short WindowColor;
} WINDOWSTYLE;

//クイズを保存配列
typedef struct
{
	string Question;
	vector<string> Anwsers;
	int RightAnswer;
	int InputedAnswer;
} QUIZZ;

//windowを作る
int makeWindow(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, WINDOWSTYLE windowStyle);
//クイズの問題を表す
int showQuestion(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, WINDOWSTYLE windowStyle, string question);
//クイズの選択肢を表す
void showList(HANDLE hwindow, vector<string> list, int index, BEGINPOSITION bposition, WINDOWSIZE wsize);
//正解とか表す
int showResult(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, WINDOWSTYLE windowStyle, vector<int> result);
//入力を取る
int getinput(int * index, int indexsize);


int main()
{
	//set the console size
	system("mode con cols=143 lines=50  ");
	system("CLS");
	//--read quizz
	vector<QUIZZ> vQuizzs(quizznum);

	//open the quizz file
	fstream fQuizz("quizz.txt");
	if (!fQuizz.is_open())
	{
		cout << "oh holy smokes!";
		getchar();
		return 0;
	}

	for (auto iter = vQuizzs.begin(); iter != vQuizzs.end(); iter++)
	{
		if (fQuizz.eof())
		{
			break;
		}
		fQuizz >> iter->Question;
		for (auto j = 0; j < 3; j++)
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
	//make the main window
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
	vector<int> vResultPoint;

	BEGINPOSITION ResultWindowBeginPosition = { 40,20 };
	WINDOWSIZE ResulteWindowSize = { 6,3,1 };
	WINDOWSTYLE ResultWindowStyle = { "☆",0xB };


	for (auto iter = vQuizzs.begin(); iter != vQuizzs.end(); iter++)
	{
		auto index = 0;
		auto result = 0;
		makeWindow(hWindow, MainWindowBeginPosition, MainWindowSize, MainWindowStyle);
		//show question
		showQuestion(hWindow, MainWindowBeginPosition, MainWindowSize, MainWindowStyle, iter->Question);
		//show answer
		while (true)
		{
			showList(hWindow, iter->Anwsers, index, MainWindowBeginPosition, MainWindowSize);
			result = getinput(&index, iter->Anwsers.size());
			if (result == ENTER)
			{
				//makeWindow(hWindow, ResultWindowBeginPosition, ResulteWindowSize, ResultWindowStyle);

				iter->InputedAnswer = index + 1;
				if (iter->InputedAnswer == iter->RightAnswer)
				{
					//cout << "正解！";
					vResultPoint.push_back(1);
				}
				else
				{
					//cout << "残念！";
					vResultPoint.push_back(0);
				}
				//Sleep(1500);
				//rewind("stdin");
				system("cls");
				break;
			}
		}
	}

	makeWindow(hWindow, MainWindowBeginPosition, MainWindowSize, MainWindowStyle);
	showResult(hWindow, MainWindowBeginPosition, MainWindowSize, MainWindowStyle, vResultPoint);

	getchar();
	return 0;
}

//make the window by position size type
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

int showResult(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, WINDOWSTYLE windowStyle, vector<int> result)
{
	ofstream fresuilt("result.txt");
	COORD cursorPosition;
	cursorPosition.X = bposition.x + wsize.wide * windowStyle.WindowFrameStyle.length();
	cursorPosition.Y = bposition.y + wsize.wide;
	SetConsoleCursorPosition(hwindow, cursorPosition);
	SetConsoleTextAttribute(hwindow, 0xB);
	int point = 0;
	int num = 0;
	for (auto i = result.begin(); i!=result.end(); i++)
	{
		num++;
		if (*i==1)
		{
			cout << "第"<< num <<"問:RIGHT!";
			fresuilt << "第" << num << "問:RIGHT!" << endl;
			point++;
		}
		else
		{
			cout << "第"<< num <<"問:WRONG!";
			fresuilt << "第" << num << "問:WRONG!" << endl;
		}
		cursorPosition.X = bposition.x + wsize.wide * windowStyle.WindowFrameStyle.length();
		cursorPosition.Y++;
		SetConsoleCursorPosition(hwindow, cursorPosition);

	}
	cout << "得点:" << point<<"!";
	fresuilt << "得点:" << point<<"!";
	return point;
}

//問題の文字を表す
int showQuestion(HANDLE hwindow, BEGINPOSITION bposition, WINDOWSIZE wsize, WINDOWSTYLE windowStyle, string question)
{
	COORD cursorPosition;
	cursorPosition.X = bposition.x + wsize.wide * windowStyle.WindowFrameStyle.length();
	cursorPosition.Y = bposition.y + wsize.wide;
	SetConsoleCursorPosition(hwindow, cursorPosition);
	SetConsoleTextAttribute(hwindow, 0xB);
	if (question.length() < 2*(wsize.x - 2 * wsize.wide * windowStyle.WindowFrameStyle.length()))
	{
		cout << question;
		cursorPosition.X = bposition.x + wsize.wide * windowStyle.WindowFrameStyle.length();
		cursorPosition.Y++;
		SetConsoleCursorPosition(hwindow, cursorPosition);
	}
	else
	{
		int snum = question.length() / (wsize.x - 2 * wsize.wide * windowStyle.WindowFrameStyle.length());
		int lostnum = question.length() % (wsize.x - 2 * wsize.wide * windowStyle.WindowFrameStyle.length());
		for (size_t i = 0; i < snum; i++)
		{
			cout << question.substr(i * (wsize.x - 2 * wsize.wide * windowStyle.WindowFrameStyle.length()), wsize.x - 2 * wsize.wide * windowStyle.WindowFrameStyle.length());

			cursorPosition.X = bposition.x + wsize.wide * windowStyle.WindowFrameStyle.length();
			cursorPosition.Y++;
			SetConsoleCursorPosition(hwindow, cursorPosition);
		}
		if (lostnum != 0)
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

//選択肢を表す
void showList(HANDLE hwindow, vector<string> list, int index, BEGINPOSITION bposition, WINDOWSIZE wsize)
{
	COORD cursorPosition;
	SetConsoleTextAttribute(hwindow, FOREGROUND_GREEN | 0x8);
	cursorPosition.X = bposition.x + wsize.x / 3;
	cursorPosition.Y = bposition.y + 6;
	SetConsoleCursorPosition(hwindow, cursorPosition);
	for (auto i = 0; i < list.size(); i++)
	{
		if (i == index)//もし選んだら、赤になる
		{
			SetConsoleTextAttribute(hwindow, FOREGROUND_RED | 0x8);
			cursorPosition.X = bposition.x + wsize.x / 3;
			cursorPosition.Y++;
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

//input を取る関数
int getinput(int * index, int indexsize)
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
