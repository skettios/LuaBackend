#include <thread>
#include <iostream>
#include <windows.h>
#include <filesystem>
#include <MemoryLib.h>

#include <LuaBackend.h>
#include <future>

using namespace std;

LuaBackend* _backend = nullptr;

void _execute(std::future<void> futureObj)
{
	while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		for (auto _script : _backend->loadedScripts)
			if (_script->frameFunction)
				_script->frameFunction();
	}
}

int main()
{
	cout << "=== LuaBackend | v0.01 ALPHA ===" << "\n";
	cout << "================================" << "\n";

	std::string _currPath = std::filesystem::current_path().u8string();
	_currPath.append("\\emu\\pcsx2.exe");

	cout << "MESSAGE: Found linked executable! Running..." << "\n";
	MemoryLib::ExecuteProcess(_currPath);

	cout << "MESSAGE: Hooking and initializing the backend..." << "\n";
	_backend = new LuaBackend();

	cout << "MESSAGE: Executing initialization event handlers..." << "\n\n";
	for (auto _script : _backend->loadedScripts)
		if (_script->initFunction)
			_script->initFunction();
	
	std::promise<void> exitSignal;
	std::future<void> futureObj = exitSignal.get_future();
	std::thread _frameThread(&_execute, std::move(futureObj));

	while (true)
	{
		string _output;
		getline(std::cin, _output);
		std::transform(_output.begin(), _output.end(), _output.begin(), ::tolower);

		if (_output == "exit")
		{
			exitSignal.set_value();
			_frameThread.join();
			break;
		}
	}

}