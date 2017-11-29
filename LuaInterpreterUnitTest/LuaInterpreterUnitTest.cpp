// LuaInterpreterUnitTest.cpp : This project is used to test the new LuaFeature¡£
//

#include "stdafx.h"
#include "../../Library/Lua/LuaInterpreter/LuaInterpreter.h"

#pragma comment(lib, "LuaInterpreter.lib")

int main()
{
	auto pLuaInter = std::make_unique<Lua::LuaInterpreter>();
	pLuaInter->Run();
    return 0;
}

