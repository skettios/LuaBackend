#include <iostream>
#include <windows.h>
#include <filesystem>
#include <MemoryLib.h>

#include <LuaBackend.h>

int main()
{
	std::string _currPath = std::filesystem::current_path().u8string();
	_currPath.append("\\emu\\pcsx2.exe");

	std::cout << _currPath;
	MemoryLib::ExecuteProcess(_currPath);

	LuaBackend _backend = LuaBackend();

	while (true)
		_backend.loadedScripts[0]->frameFunction();

	getchar();
}