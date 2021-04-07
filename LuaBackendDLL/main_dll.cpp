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

	LuaBackend* _backend = nullptr;
	chrono::high_resolution_clock::time_point _clock;

	int __declspec(dllexport) EntryLUA(int ProcessID, HANDLE ProcessH, uint64_t TargetAddress, const char* ScriptPath)
	{
		AllocConsole();
		auto _conRedirect = freopen("CONOUT$", "w", stdout);

		cout << "======================================" << "\n";
		cout << "======= LuaBackend | v0.5 BETA =======" << "\n";
		cout << "====== Copyright 2021 - TopazTK ======" << "\n";
		cout << "======================================" << "\n";
		cout << "=== Compatible with LuaEngine v2.8 ===" << "\n";
		cout << "========== Embedded Version ==========" << "\n";
		cout << "======================================" << "\n\n";

		cout << "MESSAGE: Initializing..." << "\n";

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
		_clock = chrono::high_resolution_clock::now();

		return 0;
	}

	void __declspec(dllexport) ExecuteLUA()
	{
		auto _currTime = chrono::high_resolution_clock::now();
		auto _msTime = std::chrono::duration_cast<std::chrono::milliseconds>(_currTime - _clock).count();

		if (_msTime > 16)
		{
			for (auto _script : _backend->loadedScripts)
				if (_script->frameFunction)
					_script->frameFunction();

			_clock = chrono::high_resolution_clock::now();
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