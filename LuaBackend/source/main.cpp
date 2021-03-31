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
	cout << "==== LuaBackend | v0.1 BETA ====" << "\n";
	cout << "=== Copyright 2021 - TopazTK ===" << "\n";
	cout << "================================" << "\n";
	cout << "== Compatible with LuaEngine. ==" << "\n";
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
				bool _attachOn = false;

				if (_table.has("BIG-ENDIAN"))
				{
					string _boolGet = _table["BIG-ENDIAN"];
					transform(_boolGet.begin(), _boolGet.end(), _boolGet.begin(), ::tolower);

					_bigEndian = _boolGet == "true" ? true : false;
				}

				if (_table.has("ATTACH"))
				{
					string _boolGet = _table["ATTACH"];
					transform(_boolGet.begin(), _boolGet.end(), _boolGet.begin(), ::tolower);

					_attachOn = _boolGet == "true" ? true : false;
				}

				_currPath.append("\\" + _exec);
				
				uint64_t _baseAddress;
				std::istringstream _stream(_addr);
				_stream >> _baseAddress;

				if (_attachOn)
				{
					cout << "MESSAGE: Trying to attach to the specified executable..." << "\n";
					bool _status = MemoryLib::LatchProcess(_exec, _baseAddress, _bigEndian);

					if (_status)
						cout << "MESSAGE: Link was successful..." << "\n";

					else
					{
						cout << "\n" << "ERROR: Link failed! Aborting..." << "\n";
						cout << "Press any key to exit..." << "\n";
						cin.get();
						return -5;
					}
				}

				else
				{
					cout << "MESSAGE: Trying to run the specified executable..." << "\n";
					MemoryLib::ExecuteProcess(_currPath, _baseAddress, _bigEndian);
				}

				uint64_t _execAddress = (uint64_t)MemoryLib::FindBaseAddr(MemoryLib::_pHandle, _exec);
				MemoryLib::SetExecAddr(_execAddress);

				if (_addr.find(_exec) != std::string::npos)
				{
					string _offStr = _addr;
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

				cout << "MESSAGE: Hooking and initializing the backend..." << "\n";
				_backend = new LuaBackend(_exec);

				cout << "\n" << "MESSAGE: Executing initialization event handlers..." << "\n\n";
				for (auto _script : _backend->loadedScripts)
					if (_script->initFunction)
						_script->initFunction();

				promise<void> exitSignal;
				future<void> futureObj = exitSignal.get_future();
				std::thread _frameThread(&_execute, std::move(futureObj));

				cout << "MESSAGE: Initialization complete!" << "\n";
				cout << "MESSAGE: Type \"reload\" to reload all scripts, type \"exit\" to exit." << "\n\n";
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
						main();
					}
				}
			}

			else
			{
				cout << "ERROR: Invalid config! Field \"EXECUTABLE\" not found!" << "\n";
				cout << "Press any key to exit..." << "\n";
				cin.get();
				return -2;
			}
		}

		else
		{
			cout << "ERROR: Invalid config! Table \"CONFIG\" not found!" << "\n";
			cout << "Press any key to exit..." << "\n";
			cin.get();
			return -3;
		}
	}

	else
	{
		cout << "ERROR: Config file \"config.ini\" not found!" << "\n";
		cout << "Press any key to exit..." << "\n";
		cin.get();
		return -1;
	}
}