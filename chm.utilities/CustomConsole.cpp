#include "CustomConsole.h"

enum ConsoleColor CustomConsole::ForeColor = ColorGray;
enum ConsoleColor CustomConsole::BackColor = ColorBlack;
enum ConsoleColor CustomConsole::LastForeColor = ColorGray;
enum ConsoleColor CustomConsole::LastBackColor = ColorBlack;

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CustomConsole::ForegroundColor(enum ConsoleColor color)
{
	LastForeColor = ForeColor;
	ForeColor = color;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BackColor * 16 + ForeColor);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CustomConsole::BackgroundColor(enum ConsoleColor color)
{
	LastBackColor = BackColor;
	BackColor = color;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BackColor * 16 + ForeColor);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CustomConsole::LastState(void)
{
	ForeColor = LastForeColor;
	BackColor = LastBackColor;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BackColor * 16 + ForeColor);
}

//----------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------
void CustomConsole::Default(void)
{
	LastForeColor = ForeColor;
	LastBackColor = BackColor;
	ForeColor = ColorGray;
	BackColor = ColorBlack;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BackColor * 16 + ForeColor);
}