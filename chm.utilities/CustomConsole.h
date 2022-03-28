#pragma once
#ifndef CustomConsole_H
#define CustomConsole_H

#include <Windows.h>

enum ConsoleColor {
	ColorBlack				=  0,	 // color black.
	ColorDarkBlue			=  1,  // color dark blue.
	ColorDarkGreen		=  2,  // color dark green.
	ColorDarkCyan			=  3,  // color dark cyan(dark blue - green).
	ColorDarkRed			=  4,	 // color dark red.
	ColorDarkMagenta	=  5,	 // color dark magenta(dark purplish - red).
	ColorDarkYellow		=  6,	 // color dark yellow(ochre).
	ColorGray					=	 7,	 // color gray.
	ColorDarkGray			=	 8,	 // color dark gray.
	ColorBlue					=  9,  // color blue.
	ColorGreen				= 10,  // color green.
	ColorCyan					= 11,  // color cyan(blue - green).
	ColorRed					= 12,  // color red.
	ColorMagenta			= 13,  // color magenta(purplish - red).
	ColorYellow				= 14,  // color yellow.
	ColorWhite				= 15   // color white.
};

//------------------------------------------------------------------------------
//	ClassName  : CustomConsole
//
//  Description: .
//
//  Revision   : 07/04/2019 
//------------------------------------------------------------------------------
class CustomConsole {
	private:
		static enum ConsoleColor ForeColor;
		static enum ConsoleColor BackColor;
		static enum ConsoleColor LastForeColor;
		static enum ConsoleColor LastBackColor;

	public:
		static void ForegroundColor ( enum ConsoleColor color );
		static void BackgroundColor ( enum ConsoleColor color );
		static void LastState  ( void );
		static void Default	( void );
};
#endif 
