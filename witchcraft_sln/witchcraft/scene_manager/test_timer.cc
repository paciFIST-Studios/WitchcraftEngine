#ifndef TEST_TIMER_CC
#define TEST_TIMER_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "timer.h"
		
		TEST_CASE("cTimer::cTimer()")
		{
			cTimer * ptr;
			REQUIRE_NOTHROW(ptr = &cTimer());

			REQUIRE(ptr != nullptr);

			REQUIRE(ptr->_id == cTimer::UNINIT_ID);
			REQUIRE(ptr->_start_time == cTimer::UNINIT_START_TIME);
			REQUIRE(ptr->_interval_length == cTimer::UNINIT_INTERVAL_LEN);
			REQUIRE(ptr->_time_period_elapsed == cTimer::UNINIT_TIME_ELAPSED);
		}

		TEST_CASE("cTimer::start()")
		{
			// can call start method
			auto t1 = cTimer();
			REQUIRE_NOTHROW(t1.start());
		}

		TEST_CASE("cTimer::update()")
		{
			auto t3 = &cTimer();
			t3->_interval_length = 100; // ms

			t3->start();
			for (int i = 0; i < 100; i++)
			{
				t3->update();
				Sleep(25);

				if (t3->_time_period_elapsed)
					break;
			}
			REQUIRE(t3->_time_period_elapsed);
		}


	#endif // RUN_UNIT_TESTS

#endif // !TEST_TIMER_CC
