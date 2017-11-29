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

#define TEST_UNIT_START testUnits.push_back(std::move(\
	[](Lua::LuaInterpreter* pluainter, std::string& unitName) -> bool {

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

	totalCount = testUnits.size();
	printf("Summarize:\n");
	printf("TestCount\tsuccess\t\tfailed\n");
	printf("%d\t\t%d\t\t%d\n", totalCount, success, totalCount - success);
	getchar();

	return 0;
}

void execute()
{
	pLuaInter = std::move(std::make_unique<Lua::LuaInterpreter>());
	pLuaInter->DoFile("Init.lua");
	addTestUnit();
	runTest();
	pLuaInter->Run();
}

void runTest()
{
	std::string unitName;
	for (auto & testUnit : testUnits)
	{
		bool result = testUnit(pLuaInter.get(), unitName);
		success += result;
		printf("Unit Test:\t%s\t\t\t\t%s", unitName.c_str(), result ? "success" : "failed");
	}
}

void addTestUnit()
{
	TEST_UNIT_START
		pluainter->GetGlobal("test_GetGlobal");
		unitName = "test get unit name";
		int isNum;
		int itg = lua_tointegerx(pluainter->m_L, -1, &isNum);
		if (isNum && itg == 6)
		{
			return true;
		}
		else
		{
			return false;
		}
	TEST_UNIT_END;

	TEST_UNIT_START
		pluainter->GetGlobal("test_ToNumberAndPop_1");
		unitName = "test to number and pop";
		int itg = pluainter->ToIntegerAndPop<int>();
		if (itg == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	TEST_UNIT_END; 
	
	TEST_UNIT_START
		pluainter->GetGlobal("test_ToNumberAndPop_2");
		unitName = "test to number and pop inner type";
		int itg = pluainter->ToIntegerAndPop<int>();
		if (itg == 2)
		{
			return true;
		}
		else
		{
			return false;
		}
	TEST_UNIT_END;
}
