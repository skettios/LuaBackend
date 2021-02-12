#include "LuaBackend.h"

LuaBackend::LuaBackend()
{
	loadedScripts.clear();
	string _currPath = std::filesystem::current_path().u8string().append("\\scripts");

	for (auto _path : filesystem::directory_iterator(_currPath))
	{
		LuaScript* _script = new LuaScript();

		_script->luaState.open_libraries
		(
			lib::base,
			lib::package,
			lib::coroutine,
			lib::string,
			lib::os,
			lib::math,
			lib::table,
			lib::io,
			lib::bit32,
			lib::utf8
		);

		SetFunctions(&_script->luaState);

		string _luaPath = _currPath;
		_luaPath.append("\\io_packages\\?.lua");

		string _dllPath = _currPath;
		_dllPath.append("\\io_packages\\?.dll");

		_script->luaState["package"]["path"] = _luaPath;
		_script->luaState["package"]["cpath"] = _dllPath;

		string _filePath(_path.path().u8string());

		if (_filePath.find(".lua") != std::string::npos) {
			cout << "\nLuaEngine: Found script! Initializing...\n";
			auto _result = _script->luaState.script_file(_filePath);

			_script->initFunction = _script->luaState["_OnInit"];
			_script->frameFunction = _script->luaState["_OnFrame"];

			if (!_script->initFunction && !_script->frameFunction)
			{
				cout << "\nLuaEngine: None of the needed functions exist or all of them have errors.\n";
				cout << L"LuaEngine: Initialization of this script cannot continue...\n";
				return;
			}

			if (!_script->initFunction)
				cout << "LuaEngine: The \"_OnInit\" function either has errors or does not exist.\n";

			if (!_script->frameFunction)
				cout << "LuaEngine: The \"_OnFrame\" function either has errors or does not exist.\n";

			cout << "LuaEngine: Initialization Successful!\n";
			loadedScripts.push_back(_script);
		}
	}
}

void LuaBackend::SetFunctions(LuaState* _state)
{
	_state->set_function("ReadShort", MemoryLib::ReadShort);
	_state->set_function("ReadInt", MemoryLib::ReadInt);
	_state->set_function("ReadArray", MemoryLib::ReadBytes);

	_state->set_function("Print", sol::overload(MemoryLib::Print<int>, MemoryLib::Print<std::string>));
}