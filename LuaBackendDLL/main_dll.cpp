#include <chrono>
#include <iostream>
#include <windows.h>
#include <filesystem>

#include <MemoryLib.h>
#include <LuaBackend.h>

#include <mIni/ini.h>

extern "C"
{
	using namespace std;
	using namespace mINI;

	bool _funcOneState = true;
	bool _funcTwoState = true;

	string _scrPath;

	bool _showConsole = false;
	bool _requestedReset = false;
	LuaBackend* _backend = nullptr;

	chrono::high_resolution_clock::time_point _sClock;
	chrono::high_resolution_clock::time_point _msClock;

	void ResetLUA()
	{
		cout << "MESSAGE: Reloading...\n";

		_backend = new LuaBackend(_scrPath.c_str());

		if (_backend->loadedScripts.size() == 0)
			cout << "MESSAGE: No scripts found! Reload halted." << "\n";

		cout << "MESSAGE: Executing initialization event handlers..." << "\n\n";

		for (auto _script : _backend->loadedScripts)
			if (_script->initFunction)
				_script->initFunction();

		cout << "MESSAGE: Reload complete!" << "\n";

		_msClock = chrono::high_resolution_clock::now();
		_sClock = chrono::high_resolution_clock::now();

		_requestedReset = false;
	}

	int __declspec(dllexport) EntryLUA(int ProcessID, HANDLE ProcessH, uint64_t TargetAddress, const char* ScriptPath)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);

		ShowWindow(GetConsoleWindow(), SW_HIDE);

		cout << "======================================" << "\n";
		cout << "======= LuaBackend | v0.6 BETA =======" << "\n";
		cout << "====== Copyright 2021 - TopazTK ======" << "\n";
		cout << "======================================" << "\n";
		cout << "=== Compatible with LuaEngine v2.8 ===" << "\n";
		cout << "========== Embedded Version ==========" << "\n";
		cout << "======================================" << "\n\n";

		cout << "MESSAGE: Initializing..." << "\n";
		_scrPath = string(ScriptPath);

		MemoryLib::ExternProcess(ProcessID, ProcessH, TargetAddress);
		_backend = new LuaBackend(ScriptPath);

		if (_backend->loadedScripts.size() == 0)
		{
			cout << "MESSAGE: No scripts found! Initialization halted." << "\n";
			return -1;
		}

		cout << "MESSAGE: Executing initialization event handlers..." << "\n\n";

		for (auto _script : _backend->loadedScripts)
			if (_script->initFunction)
				_script->initFunction();

		cout << "MESSAGE: Initialization complete!" << "\n";
		cout << "MESSAGE: Press 'F1' to reload all scripts, press 'F2' to toggle the console." << "\n";

		_msClock = chrono::high_resolution_clock::now();
		_sClock = chrono::high_resolution_clock::now();

		return 0;
	}

	void __declspec(dllexport) ExecuteLUA()
	{
		auto _currTime = chrono::high_resolution_clock::now();
		auto _msTime = std::chrono::duration_cast<std::chrono::milliseconds>(_currTime - _msClock).count();
		auto _sTime = std::chrono::duration_cast<std::chrono::milliseconds>(_currTime - _sClock).count();

		if (_msTime > 16)
		{
			if (GetKeyState(VK_F2) & 0x8000 && _funcTwoState)
			{
				if (_showConsole)
				{
					ShowWindow(GetConsoleWindow(), SW_HIDE);
					_showConsole = false;
				}

				else
				{
					ShowWindow(GetConsoleWindow(), SW_RESTORE);
					_showConsole = true;
				}

				_sTime = 0;
				_funcTwoState = false;
				_sClock = chrono::high_resolution_clock::now();
			}

			if (GetKeyState(VK_F1) & 0x8000 && _funcOneState)
			{
				_requestedReset = true;

				_sTime = 0;
				_funcOneState = false;
				_sClock = chrono::high_resolution_clock::now();
			}

			for (auto _script : _backend->loadedScripts)
				if (_script->frameFunction)
					_script->frameFunction();

			if (_requestedReset)
				ResetLUA();

			_msClock = chrono::high_resolution_clock::now();
		}

		if (_sTime > 250)
		{
			_funcOneState = true;
			_funcTwoState = true;
			_sClock = chrono::high_resolution_clock::now();
		}
	}

	bool __declspec(dllexport) CheckLUA()
	{
		auto _int = MemoryLib::ReadInt(0);

		if (_int == 0)
			return false;

		return true;
	}

	int __declspec(dllexport) VersionLUA()
	{
		return 128;
	}
}