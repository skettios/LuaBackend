#include <thread>
#include <future>
#include <iostream>
#include <windows.h>
#include <filesystem>

#include <MemoryLib.h>
#include <LuaBackend.h>

#include <mIni/ini.h>

using namespace std;
using namespace mINI;
LuaBackend* _backend = nullptr;

void _execute(future<void> futureObj)
{
	while (futureObj.wait_for(chrono::milliseconds(1)) == future_status::timeout)
	{
		for (auto _script : _backend->loadedScripts)
			if (_script->frameFunction)
				_script->frameFunction();
	}
}

int main()
{
	cout << "================================" << "\n";
	cout << "=== LuaBackend | v0.01 ALPHA ===" << "\n";
	cout << "=== Copyright - TopazTK 2021 ===" << "\n";
	cout << "================================" << "\n\n";

	string _currPath = filesystem::current_path().u8string();
	string _confPath = _currPath;
	string _scriptPath = _currPath;

	_confPath.append("\\config.ini");
	_scriptPath.append("\\scripts\\");

	INIFile _iniFile(_confPath);
	INIStructure _iniStruct;

	bool _readSuccess = _iniFile.read(_iniStruct);


	if (_readSuccess)
	{
		if (_iniStruct.has("CONFIG"))
		{
			auto _table = _iniStruct["CONFIG"];

			if (_table.has("EXECUTABLE"))
			{
				auto _exec = _table["EXECUTABLE"];
				auto _addr = _table["BASE_ADDRESS"];

				bool _bigEndian = false;

				if (_table.has("BIG-ENDIAN"))
				{
					string _boolGet = _table["BIG-ENDIAN"];
					transform(_boolGet.begin(), _boolGet.end(), _boolGet.begin(), ::tolower);

					_bigEndian = _boolGet == "true" ? true : false;
				}

				_currPath.append("\\" + _exec);
				uint32_t _baseAddress = stoi(_addr.c_str(), nullptr, 16);

				cout << "MESSAGE: Found linked executable! Running..." << "\n";
				MemoryLib::ExecuteProcess(_currPath, _baseAddress, _bigEndian);
				
				if (!filesystem::exists(_scriptPath))
					filesystem::create_directory(_scriptPath);

				cout << "MESSAGE: Hooking and initializing the backend..." << "\n";
				_backend = new LuaBackend();

				cout << "\n" << "MESSAGE: Executing initialization event handlers..." << "\n\n";
				for (auto _script : _backend->loadedScripts)
					if (_script->initFunction)
						_script->initFunction();

				promise<void> exitSignal;
				future<void> futureObj = exitSignal.get_future();
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

			else
				cout << "ERROR: Invalid config! Field \"EXECUTABLE\" not found!" << "\n";
		}

		else
			cout << "ERROR: Invalid config! Table \"CONFIG\" not found!" << "\n";
	}

	else
		cout << "ERROR: Config file \"config.ini\" not found!" << "\n";
}