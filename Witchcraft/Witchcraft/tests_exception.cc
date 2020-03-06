#ifndef TEST_EXCEPTION_CC
#define TEST_EXCEPTION_CC

	// this header controls whether or not the tests run
	#include "unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS

	#include <string>
	#include <typeinfo>

	#include "catch.hpp"
	#include "engine\exception.h"
	
	constexpr int exceptionNumber = 1;
	const std::string exceptionDesc = "test exception description: this is a test exception";
	const std::string exceptionFileName = "";
	constexpr int exceptionLineNumber = 0;
	
	TEST_CASE("cException: Default ctor constructs exception correctly")
	{
		cException e(exceptionNumber, exceptionDesc, exceptionFileName, exceptionLineNumber);
			
		// object was constructed
		REQUIRE(&e != nullptr);

		// checking the error number
		REQUIRE(e.m_ErrorNum == exceptionNumber);
		REQUIRE(e.m_ErrorNum != e.m_LineNum);
		REQUIRE(e.m_ErrorNum != exceptionLineNumber);
		
		// checking the description
		REQUIRE(e.m_ErrorDesc == exceptionDesc);
		REQUIRE(e.m_ErrorDesc != e.m_SrcFileName);
		REQUIRE(e.m_ErrorDesc != exceptionFileName);
		
		// checking the file name
		REQUIRE(e.m_SrcFileName == exceptionFileName);
		REQUIRE(e.m_SrcFileName != e.m_ErrorDesc);
		REQUIRE(e.m_SrcFileName != exceptionDesc);
		
		// checking line number
		REQUIRE(e.m_LineNum == exceptionLineNumber);
		REQUIRE(e.m_LineNum != e.m_ErrorNum);
		REQUIRE(e.m_LineNum != exceptionNumber);

		// the human readable description is meant to come out of here,
		// and also, this is given to us from std::exception class
		REQUIRE(e.what());

		// I think this isn't supposed to throw an error, but honestly I don't even know
		REQUIRE_NOTHROW(e.~cException());
	}
	
	
	#endif // RUN_UNIT_TESTS


#endif // TEST_EXCEPTION_CC