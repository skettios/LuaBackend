#include "LuaBackend.h"
#include "CRC.h"

string _pathStr = "";

LuaBackend::LuaBackend(const char* ScrPath)
{
	loadedScripts.clear();

	for (auto _path : filesystem::directory_iterator(ScrPath))
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

		string _luaPath = ScrPath;
		_luaPath.append("\\io_packages\\?.lua");

		string _dllPath = ScrPath;
		_dllPath.append("\\io_packages\\?.dll");

		_script->luaState["package"]["path"] = _luaPath;
		_script->luaState["package"]["cpath"] = _dllPath;

		string _loadPath = ScrPath;
		_loadPath.append("\\io_load");

		_script->luaState["LOAD_PATH"] = _loadPath;
		_script->luaState["SCRIPT_PATH"] = ScrPath;
		_script->luaState["CHEATS_PATH"] = "NOT_AVAILABLE";

		string _pathFull = MemoryLib::PName;
		auto _pathExe = _pathFull.substr(_pathFull.find_last_of("\\") + 1);

		_script->luaState["ENGINE_VERSION"] = 2.81;
		_script->luaState["ENGINE_TYPE"] = "BACKEND";
		_script->luaState["GAME_ID"] = CRC::Calculate(_pathExe.c_str(), _pathExe.length(), CRC::CRC_32());

		string _filePath(_path.path().u8string());

		if (_filePath.find(".lua") != std::string::npos) 
		{
			string _luaName = _filePath.substr(_filePath.find_last_of("\\") + 1);
			cout << "\nMESSAGE: Found script \"" + _luaName + "\" Initializing...\n";

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
	_state->set_function("WriteExec", MemoryLib::WriteExec);
	_state->set_function("GetPointer", MemoryLib::GetPointer);

	_state->set_function("ReadByteA", MemoryLib::ReadByte);
	_state->set_function("ReadShortA", MemoryLib::ReadShort);
	_state->set_function("ReadIntA", MemoryLib::ReadInt);
	_state->set_function("ReadFloatA", MemoryLib::ReadFloat);
	_state->set_function("ReadBooleanA", MemoryLib::ReadBool);
	_state->set_function("ReadArrayA", MemoryLib::ReadBytes);
	_state->set_function("ReadStringA", MemoryLib::ReadString);

	_state->set_function("WriteByteA", MemoryLib::WriteByte);
	_state->set_function("WriteShortA", MemoryLib::WriteShort);
	_state->set_function("WriteIntA", MemoryLib::WriteInt);
	_state->set_function("WriteFloatA", MemoryLib::WriteFloat);
	_state->set_function("WriteBooleanA", MemoryLib::WriteBool);
	_state->set_function("WriteArrayA", MemoryLib::WriteBytes);
	_state->set_function("WriteStringA", MemoryLib::WriteString);
	_state->set_function("WriteExecA", MemoryLib::WriteExec);
	_state->set_function("GetPointerA", MemoryLib::GetPointerAbsolute);
}