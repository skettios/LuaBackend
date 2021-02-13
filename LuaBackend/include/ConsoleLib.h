#ifndef CONSOLELIB
#define CONSOLELIB

#include <iostream>
#include <string>

class ConsoleLib
{
	public:
	template <class T>
	static void Print(T Input) { std::cout << "OUTPUT: " << Input << "\n"; }
};

#endif