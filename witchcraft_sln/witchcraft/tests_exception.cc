#ifndef TEST_EXCEPTION_CC
#define TEST_EXCEPTION_CC

	// this header controls whether or not the tests run
	#include "unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS

		#include <string>
		#include <typeinfo>

		#include "catch.hpp"
		#include "engine\exception.h"
		
		constexpr int EXCEPTION_NUMBER = 1;
		const std::string EXCEPTION_DESCRIPTION = "test exception description: this is a test exception";
		const std::string EXCEPTION_FILE_NAME = "";
		constexpr int EXCEPTION_LINE_NUMBER = 0;
		
		TEST_CASE("qException: Default ctor constructs exception correctly")
		{
			qException e(
				  EXCEPTION_NUMBER
				, EXCEPTION_DESCRIPTION
				, EXCEPTION_FILE_NAME
				, EXCEPTION_LINE_NUMBER
			);
				
			// object was constructed
			REQUIRE(&e != nullptr);

			// checking the error number
			REQUIRE(e._error_id == EXCEPTION_NUMBER);
			REQUIRE(e._error_id != e._line_number);
			REQUIRE(e._error_id != EXCEPTION_LINE_NUMBER);
			
			// checking the description
			REQUIRE(e._error_description == EXCEPTION_DESCRIPTION);
			REQUIRE(e._error_description != e._source_file_name);
			REQUIRE(e._error_description != EXCEPTION_FILE_NAME);
			
			// checking the file name
			REQUIRE(e._source_file_name == EXCEPTION_FILE_NAME);
			REQUIRE(e._source_file_name != e._error_description);
			REQUIRE(e._source_file_name != EXCEPTION_DESCRIPTION);
			
			// checking line number
			REQUIRE(e._line_number == EXCEPTION_LINE_NUMBER);
			REQUIRE(e._line_number != e._error_id);
			REQUIRE(e._line_number != EXCEPTION_NUMBER);

			// returns _error_description
			REQUIRE(e.what());

			// the human readable description is meant to come out of here,
			// and also, this is given to us from std::exception class
			REQUIRE(e._readable_message.length() > 0);
		}
		
		TEST_CASE("qException: dtor")
		{
			qException e(EXCEPTION_NUMBER, EXCEPTION_DESCRIPTION, EXCEPTION_FILE_NAME, EXCEPTION_LINE_NUMBER);

			// I think this isn't supposed to throw an error, but honestly I don't even know
			REQUIRE_NOTHROW(e.~qException());

			// after the delete, this sould get reset
			REQUIRE(e._readable_message.length() == 0);
		}

	#endif // RUN_UNIT_TESTS

#endif // TEST_EXCEPTION_CC
