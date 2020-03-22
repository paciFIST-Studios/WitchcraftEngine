#ifndef TEST_TIMER_CC
#define TEST_TIMER_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "timer.h"
		
		TEST_CASE("cTimer::cTimer()")
		{
			REQUIRE_NOTHROW(cTimer());
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_TIMER_CC
