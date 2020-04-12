#ifndef TEST_TIMER_CC
#define TEST_TIMER_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "timer.h"
		
		TEST_CASE(" TickTimer:: ctor")
		{
			unsigned int const test_value = 20;
			bool time_elapsed = false;
			auto timer = &TickTimer();
			REQUIRE(timer != nullptr);

			timer->set_time_length(test_value);
			timer->start();
			while (true)
			{
				if (timer->time_elapsed())
				{
					time_elapsed = true;
					break;
				}
			}

			REQUIRE(time_elapsed == true);
		}


		class TestTickTimer : public TickTimer
		{
		public:
			unsigned int get_started_at_time() const { return started_at_time; }
			unsigned int get_ticks_to_wait() const { return ticks_to_wait; }
		};

		TEST_CASE(" TestTickTimer:: ctor")
		{
			auto timer = TestTickTimer();
			REQUIRE(timer.get_started_at_time() == uninit::UINT);
			REQUIRE(timer.get_ticks_to_wait() == uninit::UINT);

			REQUIRE_NOTHROW(timer.set_time_length(uninit::UINT));
			REQUIRE_NOTHROW(timer.reset());
			REQUIRE_NOTHROW(timer.start());
			REQUIRE_NOTHROW(timer.time_elapsed());
		}

		TEST_CASE(" TestTickTimer:: set_time_length")
		{
			unsigned int const test_value = 20;
			auto timer = TestTickTimer();
			REQUIRE(timer.get_started_at_time() == uninit::UINT);
			REQUIRE(timer.get_ticks_to_wait() == uninit::UINT);
			timer.set_time_length(test_value);
			REQUIRE(timer.get_ticks_to_wait() == test_value);
		}

		TEST_CASE(" TestTickTimer:: reset")
		{
			unsigned int const test_value = 20;
			auto timer = TestTickTimer();
			REQUIRE(timer.get_started_at_time() == uninit::UINT);
			REQUIRE(timer.get_ticks_to_wait() == uninit::UINT);
			timer.set_time_length(test_value);
			REQUIRE(timer.get_ticks_to_wait() == test_value);
			timer.start();
			auto start_time = timer.get_started_at_time();
			REQUIRE(start_time != uninit::UINT);
			REQUIRE(start_time > 0);
			timer.reset();
			REQUIRE(timer.get_started_at_time() == 0);
			REQUIRE(timer.get_ticks_to_wait() == 0);
		}

		TEST_CASE(" TestTickTimer:: time_elapsed")
		{
			unsigned int const test_value = 20;
			auto timer = TestTickTimer();
			REQUIRE(timer.get_started_at_time() == uninit::UINT);
			REQUIRE(timer.get_ticks_to_wait() == uninit::UINT);
			timer.set_time_length(test_value);
			REQUIRE(timer.get_ticks_to_wait() == test_value);
			timer.start();

			while (true)
			{
				if (timer.time_elapsed())
				{
					REQUIRE(true);
					break;
				}
			}
			REQUIRE(true);
		}


		struct CallbackTestObject
		{
		public:
			bool success;
			void callback() { success = true; }
		};

		TEST_CASE(" CallbackTimer:: ctor")
		{
			unsigned int const test_value = 20;
			auto cbo = &CallbackTestObject();
			cbo->success = false;
			auto cb_timer = CallbackTimer();

			std::function<void()> fn = std::bind(&CallbackTestObject::callback, cbo);
			cb_timer.set_callback(fn);
			cb_timer.set_time_length(test_value);

			REQUIRE(cbo->success == false);
			cb_timer.start();
			while (true)
			{
				cb_timer.update();
				if (cb_timer.time_elapsed())
				{
					break;
				}
			}
			
			REQUIRE(cbo->success);
		}


	#endif // RUN_UNIT_TESTS

#endif // !TEST_TIMER_CC
