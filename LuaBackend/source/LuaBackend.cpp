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
			cout << "\nMESSAGE: Found script! Initializing...\n";
			auto _result = _script->luaState.script_file(_filePath);

			_script->initFunction = _script->luaState["_OnInit"];
			_script->frameFunction = _script->luaState["_OnFrame"];

			if (!_script->initFunction && !_script->frameFunction)
			{
				cout << "\nERROR: No event handlers exist or all of them have errors.\n";
				cout << L"ERROR: Initialization of this script cannot continue...\n";
				return;
			}

			if (!_script->initFunction)
				cout << "WARNING: The event handler for initialization either has errors or does not exist.\n";

			if (!_script->frameFunction)
				cout << "WARNING: The event handler for framedraw either has errors or does not exist.\n";

			cout << "MESSAGE: Initialization of this script was successful!\n";
			loadedScripts.push_back(_script);
		}
	}
}

void LuaBackend::SetFunctions(LuaState* _state)
{
	_state->set_function("ReadByte", MemoryLib::ReadByte);
	_state->set_function("ReadShort", MemoryLib::ReadShort);
	_state->set_function("ReadInt", MemoryLib::ReadInt);
	_state->set_function("ReadFloat", MemoryLib::ReadFloat);
	_state->set_function("ReadBoolean", MemoryLib::ReadBool);
	_state->set_function("ReadArray", MemoryLib::ReadBytes);
	_state->set_function("ReadString", MemoryLib::ReadString);

	_state->set_function("WriteByte", MemoryLib::WriteByte);
	_state->set_function("WriteShort", MemoryLib::WriteShort);
	_state->set_function("WriteInt", MemoryLib::WriteInt);
	_state->set_function("WriteFloat", MemoryLib::WriteFloat);
	_state->set_function("WriteBoolean", MemoryLib::WriteBool);
	_state->set_function("WriteArray", MemoryLib::WriteBytes);
	_state->set_function("WriteString", MemoryLib::WriteString);

	_state->set_function("Print", sol::overload(ConsoleLib::Print<int>, ConsoleLib::Print<std::string>));
}