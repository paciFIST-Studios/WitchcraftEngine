#ifndef GET_LATEST_LOG_STR__TST_CC
#define GET_LATEST_LOG_STR__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "get_latest_log_str.h"
		#include "utility.h"
		#include "../string_constants.h"

		TEST_CASE(" get_latest_log_str()")
		{
			// todo: tests here			
			//REQUIRE(false);
		}

	#endif // RUN_UNIT_TESTS

#endif // !GET_LATEST_LOG_STR__TST_CC
