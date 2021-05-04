#ifndef LUABACKEND
#define LUABACKEND

#include <iostream>
#include <filesystem>
#include <sol/sol.hpp>

#include <MemoryLib.h>
#include <ConsoleLib.h>
#include <Operator32Lib.h>
#include <DCInstance.h>



using namespace sol;
using namespace std;

using LuaState = sol::state;
using LuaFunction = sol::safe_function;

class LuaBackend
{
	public:
		struct LuaScript
		{
			LuaState luaState;
			LuaFunction initFunction;
			LuaFunction frameFunction;
		};

		std::vector<LuaScript*> loadedScripts;
		int frameLimit;

		static int ExceptionHandle(lua_State*, sol::optional<const std::exception&>, sol::string_view);
		void SetFunctions(LuaState*);
		LuaBackend(const char*);
};

#endif