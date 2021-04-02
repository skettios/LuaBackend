#ifndef CONSOLELIB
#define CONSOLELIB

#include <iostream>
#include <string>

class ConsoleLib
{
	public:
	static void Print(const char* Text)
	{
		printf(Text);
		printf("\n");
	}

	static void ColorPrint(const char* Text, int Color) 
	{ 
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, Color);
		printf(Text);
		printf("\n");
		SetConsoleTextAttribute(hConsole, 15);
	}
};

#endif