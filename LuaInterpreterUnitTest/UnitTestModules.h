#pragma once
#include <functional>
#include <string>
#include "../../Library/MyTools/MyAssert.h"

#define DECLARE_TEST_UNITS\
	static std::vector<std::function<bool(std::string& unitName)>>\
	testUnits;\
	static unsigned int totalCount	= 0;\
	static unsigned int	success		= 0;

#define TEST_UNIT_START(UnitName) testUnits.push_back(std::move(\
		[](std::string& unitName) -> bool {\
			unitName = UnitName;

#define TEST_UNIT_END }))


namespace TestUnit 
{

// the user should implements this function 
// to get some previous work for test.
// such as initialize the tested variable.
// there is no way to pass arguments between the functions,
// so just use the global variable.
void GetReady();

// user should implemment this function
// to add the test units,
// for simple using , just 
// use the macro 'TEST_UNIT_START(UnitName)' and 
// 'TEST_UNIT_END'.
// Inside the two marco , there is a lambda,
// the only thing that you will concen about is the return 
// of the lambda.
// The return value should be a boolean, which indicate
// whether the unit test success or not.
// For example:
//		TEST_UNIT_START("a test will always success")
//			...do the job...
//			return true
//		TEST_UNIT_END
void AddTestUnit();
void RunTest();
void Summary();

inline void testMain()
{
	GetReady();
	AddTestUnit();
	RunTest();
	Summary();
}

}// namespace TestUnit