// TryUnitTestModule.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../Library/MyTools/UnitTestModules.h"
#include "../../Library/Lua/LuaModuls/LuaMeshData/LuaMeshDataStruct.h"
#include "../../Library/Lua/LuaInterpreter/LuaInterpreter.h"
#include "../../Library/MyTools/MyAssert.h"
#include "../../Library/MyTools/Formater.h"

#pragma comment(lib, "LuaInterpreter.lib")

DECLARE_TEST_UNITS;

static auto GLuaInterpreter = std::make_unique<Lua::LuaInterpreter>();

#define TEST_TARGET GLuaInterpreter

// this function is declared in the UnitTestModuls.h,
// we should implements it here.
void TestUnit::GetReady()
{
	TEST_TARGET->DoFile("Init.lua");
}

// this function is declared int the UnitTestModuls.h
// we should implements it here
void TestUnit::AfterTest()
{
	TEST_TARGET->Run();
}

void TestUnit::AddTestUnit()
{
	TEST_UNIT_START("doFile is called successfully, \
		if you can see this information")
		return true;
	TEST_UNIT_END;

	TEST_UNIT_START("test get global name")
		TEST_TARGET->GetGlobal("test_GetGlobal");
		int isNum;
		int itg = lua_tointegerx(TEST_TARGET->m_L, -1, &isNum);
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
		int itg = TEST_TARGET->ToIntegerAndPop<int>();
		return itg == 3;
	TEST_UNIT_END;

	TEST_UNIT_START("test to integer and pop inner type")
		TEST_TARGET->GetGlobal("test_ToIntegerAndPop_2");
		int itg = TEST_TARGET->ToIntegerAndPop<int>();
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
		int stackSize;
		TEST_TARGET->Foreach(
			[&error](LUA_INTERPRETER_FOREACH_LAMBDA_ARGS){
				// @ pLuaInter
				int itg = luaInterForeach->ToIntegerAndPop<int>();
				if (keyIsNumber)
				{
					switch (keyItg)
					{
					case 1:
						error += 4 != itg;
						break;

					case 2:
						error += 5 != itg;
						break;

					case 3:
						error += 6 != itg;
						break;
					default:
						++error;
						break;
					}
					//DEBUG_MESSAGE("key index: %d\n", keyItg);
				}
				else
				{
					if (0 == strcmp(keyStr, "a"))
					{
						error += 1 != itg;
					}
					else if (0 == strcmp(keyStr, "b"))
					{
						error += 2 != itg;
					}
					else if (0 == strcmp(keyStr, "c"))
					{
						error += 3 != itg;
					}
					else
					{
						++error;
					}
					//DEBUG_MESSAGE("key field: %s\n", keyStr);
				}
	
			}); // end foreach
		return error == 0;
	TEST_UNIT_END;
	
	TEST_UNIT_START("traverse table use easy marco")
		TEST_TARGET->GetGlobal("test_table");
		int error = 0;
		TEST_TARGET->Foreach(
			FOREACH_START
				// @ pLuaInter
				int itg = luaInterForeach->ToIntegerAndPop<int>();
				if (keyIsNumber)
				{
					switch (keyItg)
					{
					case 1:
						error += 4 != itg;
						break;

					case 2:
						error += 5 != itg;
						break;

					case 3:
						error += 6 != itg;
						break;
					default:
						++error;
						break;
					}
					//DEBUG_MESSAGE("key index: %d\n", keyItg);
				}
				else
				{
					if (0 == strcmp(keyStr, "a"))
					{
						error += 1 != itg;
					}
					else if (0 == strcmp(keyStr, "b"))
					{
						error += 2 != itg;
					}
					else if (0 == strcmp(keyStr, "c"))
					{
						error += 3 != itg;
					}
					else
					{
						++error;
					}
					//DEBUG_MESSAGE("key field: %s\n", keyStr);
				}	
			FOREACH_END
			); // lua::foreach end
		return error == 0;
	TEST_UNIT_END;

	TEST_UNIT_START("test get userdata")
		TEST_TARGET->GetGlobal("test_userdata_meshdata");
		LuaPointerContainer<Lua::MeshData> * pmd = 
			TEST_TARGET->ToUserDataAndClear<LuaPointerContainer<Lua::MeshData>>(
				Lua::LuaMeshDataMetatableName);
		int error = 0;
		// remind that all the vector here contain a none placeholder
		// which stand for the meanless component.
		// so the avalible size should substract one
		error += 1 != ( pmd->pointer->Positions	.size() - 1 );
		error += 2 != ( pmd->pointer->Texcoords	.size() - 1 );
		error += 3 != ( pmd->pointer->Normals	.size() - 1 );
		error += 4 != ( pmd->pointer->TangentUs	.size() - 1 );
		error += 5 != ( pmd->pointer->Vertices	.size() - 1 );
		error += 6 != ( pmd->pointer->Indices32	.size() - 1 );
		return error == 0;
	TEST_UNIT_END;

	TEST_UNIT_START("test get userdata inner type")
		TEST_TARGET->GetGlobal("test_userdata_meshdata");
		LuaPointerContainer<Lua::MeshData> * pmd = nullptr;
		TEST_TARGET->ToUserDataAndClear<LuaPointerContainer<Lua::MeshData>>(
			Lua::LuaMeshDataMetatableName, &pmd);
		int error = 0;
		// remind that all the vector here contain a none placeholder
		// which stand for the meanless component.
		// so the avalible size should substract one
		error += 1 != ( pmd->pointer->Positions	.size() - 1 );
		error += 2 != ( pmd->pointer->Texcoords	.size() - 1 );
		error += 3 != ( pmd->pointer->Normals	.size() - 1 );
		error += 4 != ( pmd->pointer->TangentUs	.size() - 1 );
		error += 5 != ( pmd->pointer->Vertices	.size() - 1 );
		error += 6 != ( pmd->pointer->Indices32	.size() - 1 );
		return error == 0;
	TEST_UNIT_END;

	TEST_UNIT_START("test push integer")
		TEST_TARGET->PushInteger(111);
		return 111 == TEST_TARGET->ToIntegerAndPop();
		TEST_UNIT_END;

	TEST_UNIT_START("test push number")
		TEST_TARGET->PushNumber(2.22);
		return 10e-8 > (2.22 - TEST_TARGET->ToNumberAndPop());
	TEST_UNIT_END;

	TEST_UNIT_START("test push integer")
		const char * pts = "unique_test~!@#$%^&*()\n\t";
		Formater<256> buffer;
		TEST_TARGET->PushString(pts);
		TEST_TARGET->ToStringAndClear<buffer.Size>(buffer.bufferPointer());
		return 0 == strcmp(pts, buffer.bufferPointer());
	TEST_UNIT_END;

	TEST_UNIT_START("test get the length of the table")
		UINT length = TEST_TARGET->GetGlobal("test_table")
									->LengthOfTop<UINT>();
		return length == 3;
	TEST_UNIT_END;

	TEST_UNIT_START("test get the length of the table inner type")
		UINT length;
		TEST_TARGET->GetGlobal("test_table")
						->LengthOfTop(&length);
		return length == 3;
	TEST_UNIT_END;

	TEST_UNIT_START("test constrain stack size not to grow up")
		int preSize = lua_gettop(TEST_TARGET->m_L);
		int error = 0;
		
		int length = 0;
		TEST_TARGET->ConstainStackSizeMax(
			CONSTRAIN_START
				CONSTRAIN_TARGET->GetGlobal("test_table")
									->LengthOfTop(&length)
								->Foreach(
									FOREACH_START
										// @ pLuaInter
										int itg = luaInterForeach->ToIntegerAndPop<int>();
										if (keyIsNumber)
										{
											switch (keyItg)
											{
											case 1:
												error += 4 != itg;
												break;

											case 2:
												error += 5 != itg;
												break;

											case 3:
												error += 6 != itg;
												break;
											default:
												++error;
												break;
											}
											//DEBUG_MESSAGE("key index: %d\n", keyItg);
										}
										else
										{
											if (0 == strcmp(keyStr, "a"))
											{
												error += 1 != itg;
											}
											else if (0 == strcmp(keyStr, "b"))
											{
												error += 2 != itg;
											}
											else if (0 == strcmp(keyStr, "c"))
											{
												error += 3 != itg;
											}
											else
											{
												++error;
											}
											//DEBUG_MESSAGE("key field: %s\n", keyStr);
										}	
									FOREACH_END
									); // lua::foreach end
			CONSTRAIN_END
		);// end Lua::Constrain...Max()
		int afterSize = TEST_TARGET->GetStackSize();
		error += preSize < afterSize;
		return error == 0;
	TEST_UNIT_END;

	TEST_UNIT_START("test if else condition branch")
		// for test_ifelse_true
		int cannotBe123 = 0;
		int shouldBe456 = 0;

		// for test_ifelse_nil
		int shouldBe789 = 0;
		int cannotBe321 = 0;
		TEST_TARGET
			->GetGlobal("test_ifelse_true")
			// this Variable is always true( NOT CONDITION )
			->If(
				LUA_INTERPRETER_IF
					// if correct, here will return false, 
					// because true is never the nil.
					return CONDITION_TARGET->IsNil();
				LUA_INTERPRETER_THEN
					// if correct, this code will never be called
					THEN_TARGET
						->GetGlobal("test_if_true_then")
						->ToIntegerAndPop(&cannotBe123);
				LUA_INTERPRETER_ELSE
					// if correct, this code will be called
					ELSE_TARGET
						->GetGlobal("test_if_true_else")
						->ToIntegerAndPop(&shouldBe456);
				LUA_INTERPRETER_ENDIF
			)// end Lua::LuaInterperter::If
			->GetGlobal("test_ifelse_nil")
			// this Variable is always nil ( NOT CONDITION )
			->If(
				LUA_INTERPRETER_IF
					// if correct, here will return true, 
					// because nil is 'nil'.
					return CONDITION_TARGET->IsNil();
				LUA_INTERPRETER_THEN
					THEN_TARGET
						->GetGlobal("test_if_nil_then")
						->ToIntegerAndPop(&shouldBe789);
				LUA_INTERPRETER_ELSE
					ELSE_TARGET
						->GetGlobal("test_if_false_then")
						->ToIntegerAndPop(&cannotBe321);
				LUA_INTERPRETER_ENDIF
			);// end Lua::LuaInterepreter::If

		return	cannotBe123 != 123
			&&	shouldBe456 == 456
			&&	shouldBe789 == 789
			&&	cannotBe321 != 321;
	TEST_UNIT_END;

	TEST_UNIT_START("test run the interactive mode")
		printf("Here have enter the lua interactive mode, please enter the commands to test the interpreter.\n");
		printf("This unit test will always success\n");
		TEST_TARGET->Run();
		return true;
	TEST_UNIT_END;
}

int main()
{
	TestUnit::testMain();
    return 0;
}

