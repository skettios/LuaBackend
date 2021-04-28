#include <chrono>
#include <thread>
#include <future>
#include <iostream>
#include <windows.h>
#include <filesystem>

#include <MemoryLib.h>
#include <LuaBackend.h>

#include "config.h"

using namespace std;

int _refresh = 16;
LuaBackend* _backend = nullptr;

void EnterWait()
{
	string _output;
	getline(std::cin, _output);
	return;
}

void _execute(future<void> futureObj)
{
	while (futureObj.wait_for(chrono::milliseconds(_refresh)) == future_status::timeout)
	{
		for (auto _script : _backend->loadedScripts)
			if (_script->frameFunction)
				_script->frameFunction();
	}
}

int main()
{
	cout << "======================================" << "\n";
	cout << "========= LuaBackend | v1.00 =========" << "\n";
	cout << "====== Copyright 2021 - TopazTK ======" << "\n";
	cout << "======================================" << "\n";
	cout << "=== Compatible with LuaEngine v4.0 ===" << "\n";
	cout << "========== External Version ==========" << "\n";
	cout << "======================================" << "\n\n";

	string _currPath = filesystem::current_path().u8string();
	string _confPath = _currPath;
	string _scriptPath = _currPath;

	_confPath.append("\\config.ini");
	_scriptPath.append("\\scripts\\");

	Config config(_confPath);

	try { config.read(); }

	catch (exception& e) {
		string message = e.what();
		ConsoleLib::MessageOutput(message + '\n', 3);
		EnterWait();
		return -5;
	}

	uint64_t _baseAddress;
	_refresh = config.hzRefresh;

	std::istringstream _stream(config.baseAddr);
	_stream >> _baseAddress;

	if (config.attachOn)
	{
		ConsoleLib::MessageOutput("Trying to attach to the specified executable...\n", 0);
		bool _status = MemoryLib::LatchProcess(config.exe, _baseAddress, config.bEndian);

		if (_status)
			ConsoleLib::MessageOutput("Attachment was successful!\n", 0);

		else
		{
			ConsoleLib::MessageOutput("Could not attach to the process! Aborting...\n", 3);
			ConsoleLib::MessageOutput("Press RETURN to exit...\n", 0);

			EnterWait();
			return -5;
		}
	}

	else
	{
		ConsoleLib::MessageOutput("Trying to run the specified executable...\n", 0);

		if (MemoryLib::ExecuteProcess(_currPath, _baseAddress, config.bEndian) == -1)
		{
			ConsoleLib::MessageOutput("Could not run the executable! Aborting...\n", 0);
			ConsoleLib::MessageOutput("Press RETURN to exit...\n", 0);

			EnterWait();
			return -6;
		}
	}

	uint64_t _execAddress = (uint64_t)MemoryLib::FindBaseAddr(MemoryLib::PHandle, config.exe);

	if (config.baseAddr.find(config.exe) != std::string::npos)
	{
		auto _offStr = config.baseAddr;
		_offStr.erase(0, _offStr.find('+') + 1);

		uint64_t _offset;

		std::stringstream _stream;
		_stream << std::hex << _offStr;
		_stream >> _offset;

		_baseAddress = _execAddress + _offset;
		MemoryLib::SetBaseAddr(_baseAddress);
	}

	if (!filesystem::exists(_scriptPath))
		filesystem::create_directory(_scriptPath);

	ConsoleLib::MessageOutput("Hooking and initializing LuaEngine v4.0...\n\n", 0);
	_backend = new LuaBackend(_scriptPath.c_str());

	ConsoleLib::MessageOutput("Executing initialization event handlers...\n\n", 0);

	for (auto _script : _backend->loadedScripts)
		if (_script->initFunction)
		{
			auto _result = _script->initFunction();

			if (!_result.valid())
			{
				sol::error _err = _result;

				ConsoleLib::MessageOutput(_err.what() + '\n', 3);
				ConsoleLib::MessageOutput("Press RETURN to continue...\n", 0);

				EnterWait();
			}
		}

	promise<void> exitSignal;
	future<void> futureObj = exitSignal.get_future();
	std::thread _frameThread(&_execute, std::move(futureObj));

	ConsoleLib::MessageOutput("Initialization complete!\n", 1);
	ConsoleLib::MessageOutput("Type \"reload\" to reload all scripts, type \"exit\" to exit.\n\n", 1);

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

		else if (_output == "reload")
		{
			exitSignal.set_value();
			_frameThread.join();
			system("cls"); // This is not ideal but, too bad! This shit is hard in C++.
			main();
		}
	}
}

