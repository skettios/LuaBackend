#ifndef LUABACKEND
#define LUABACKEND

#include <iostream>
#include <filesystem>
#include <sol/sol.hpp>

#include <MemoryLib.h>
#include <ConsoleLib.h>

using namespace sol;
using namespace std;

using LuaState = sol::state;
using LuaFunction = sol::function;

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

		void SetFunctions(LuaState* _state);
		LuaBackend();
};

#endif