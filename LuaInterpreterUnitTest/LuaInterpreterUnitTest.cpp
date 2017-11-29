// LuaInterpreterUnitTest.cpp : This project is used to test the new LuaFeature¡£
//

#include "stdafx.h"
#include "../../Library/Lua/LuaInterpreter/LuaInterpreter.h"
#include "../../Library/MyTools/Formater.h"

#pragma comment(lib, "LuaInterpreter.lib")


static std::vector<
	std::function<
	bool(Lua::LuaInterpreter* pluainter, std::string& unitName)>>
	testUnits;

#define TEST_UNIT_LAMBDA_START (Lua::LuaInterpreter* pluainter, std::string& unitName) -> bool {
#define TEST_UNIT_LAMBDA_END }

#define TEST_UNIT_START(UnitName) testUnits.push_back(std::move(\
	[](Lua::LuaInterpreter* pluainter, std::string& unitName) -> bool {\
		unitName = UnitName;

// this macro is only used int the TEST_UNIT_START ~ END
#define TEST_TARGET pluainter

#define TEST_UNIT_END }))



static std::unique_ptr<Lua::LuaInterpreter> pLuaInter;

static int totalCount = 0;
static int success = 0;

void execute();
void runTest();
void addTestUnit();

int main()
{

	try
	{
		execute();
	}
	catch (SimpleException & e)
	{
		fprintf(stderr, "ERROR:%s", e.ToString().c_str());
	}

	getchar();

	return 0;
}

void execute()
{
	pLuaInter = std::move(std::make_unique<Lua::LuaInterpreter>());
	pLuaInter->DoFile("Init.lua");
	addTestUnit();
	runTest();

	totalCount = testUnits.size();
	printf("Summarize:\n");
	printf("TestCount\tsuccess\t\tfailed\n");
	printf("%d\t\t%d\t\t%d\n", totalCount, success, totalCount - success);

	pLuaInter->Run();
}

void runTest()
{
	std::string unitName;
	for (auto & testUnit : testUnits)
	{
		bool result = testUnit(pLuaInter.get(), unitName);
		success += result;
		printf("\t%s\t\t\t%s\n", result ? "success" : "failed", unitName.c_str());
	}
}

void addTestUnit()
{
	TEST_UNIT_START("doFile if the doFile is called successful, \
		you can see this information")
		return true;
	TEST_UNIT_END;

	TEST_UNIT_START("test get unit name")
		TEST_TARGET->GetGlobal("test_GetGlobal");
		int isNum;
		int itg = lua_tointegerx(pluainter->m_L, -1, &isNum);
		return (isNum && itg == 6);
	TEST_UNIT_END;

	TEST_UNIT_START("test stack size")
		int before = TEST_TARGET->GetStackSize();
		TEST_TARGET->GetGlobal("test_GetGlobal");
		before -= TEST_TARGET->GetStackSize();
		return before == -1;
	TEST_UNIT_END;

	TEST_UNIT_START("test pop")
		TEST_TARGET->GetGlobal("test_GetGlobal");
		int before = TEST_TARGET->GetStackSize();
		TEST_TARGET->Pop();
		before -= TEST_TARGET->GetStackSize();
		return before == 1;
	TEST_UNIT_END;

	TEST_UNIT_START("test to number and pop")
		TEST_TARGET->GetGlobal("test_ToNumberAndPop_1");
		float flt;
		TEST_TARGET->ToNumberAndPop<float>(&flt);
		return flt == 1;
	TEST_UNIT_END; 
	
	TEST_UNIT_START("test to number and pop inner type")
		TEST_TARGET->GetGlobal("test_ToNumberAndPop_2");
		float flt = TEST_TARGET->ToNumberAndPop<float>();
		return flt == 2;
	TEST_UNIT_END;

	TEST_UNIT_START("test to integer and pop")
		TEST_TARGET->GetGlobal("test_ToIntegerAndPop_1");
		int itg = pluainter->ToIntegerAndPop<int>();
		return itg == 3;
	TEST_UNIT_END;

	TEST_UNIT_START("test to integer and pop inner type")
		TEST_TARGET->GetGlobal("test_ToIntegerAndPop_2");
		int itg = pluainter->ToIntegerAndPop<int>();
		return itg == 4;
	TEST_UNIT_END;

	TEST_UNIT_START("test to integer and pop inner type")
		TEST_TARGET->GetGlobal("test_ToStringandClear");
		Formater<128> buffer;
		TEST_TARGET->ToStringAndClear<buffer.Size>(buffer.bufferPointer());
		return 0 == strcmp(buffer.bufferPointer(), "test_string_5");
	TEST_UNIT_END;

	TEST_UNIT_START("get field on the table(on the top of the stack)")
		TEST_TARGET->GetGlobal("test_table");
		TEST_TARGET->GetFieldOnTop("a");
		int itg = TEST_TARGET->ToIntegerAndPop<int>();
		return itg == 1;
	TEST_UNIT_END;

	TEST_UNIT_START("get index on the table(on the top of the stack)")
		TEST_TARGET->GetGlobal("test_table");
		TEST_TARGET->GetIndexOnTop(1);
		int itg = TEST_TARGET->ToIntegerAndPop<int>();
		return itg == 4;
	TEST_UNIT_END;

	TEST_UNIT_START("can isNil work")
		TEST_TARGET->GetGlobal("test_GetGlobal");
		bool shouldBeFalse = TEST_TARGET->IsNil();
		lua_pushnil(TEST_TARGET->m_L);
		bool shouldBeTrue = TEST_TARGET->IsNil();
		return Not(shouldBeFalse) && shouldBeTrue;
	TEST_UNIT_END;

	TEST_UNIT_START("traverse table")
		TEST_TARGET->GetGlobal("test_table");
		int error = 0;
		TEST_TARGET->Foreach(
			[&error](LUA_INTERPRETER_FOREACH_LAMBDA_ARGS){
				// @ pLuaInter
				int itg = luaInterForeach->ToIntegerAndPop<int>();
				if (keyIsNumber)
				{
					switch (keyItg)
					{
					case 1:
						error += 4 == itg;
						break;

					case 2:
						error += 5 == itg;
						break;

					case 3:
						error += 6 == itg;
						break;
					default:
						++error;
						break;
					}
					DEBUG_MESSAGE("key index: %d\n", keyItg);
				}
				else
				{
					if (0 == strcmp(keyStr, "a"))
					{
						error += 1 == itg;
					}
					else if (0 == strcmp(keyStr, "b"))
					{
						error += 2 == itg;
					}
					else if (0 == strcmp(keyStr, "c"))
					{
						error += 3 == itg;
					}
					else
					{
						++error;
					}
					DEBUG_MESSAGE("key field: %s\n", keyStr);
				}
	
			LUA_INTERPRETER_FOREACH_LAMBDA_END
			);
		return error;
	TEST_UNIT_END;


}
