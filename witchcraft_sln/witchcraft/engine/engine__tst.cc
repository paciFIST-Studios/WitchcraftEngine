#ifndef ENGINE__TST_CC
#define ENGINE__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../uninitialized_values.h"

		#include "engine.h"

		TEST_CASE("cEngine::ctor")
		{
			// default init 
			auto engine = cEngine();
			REQUIRE(engine.id == uninit::UINT);
			REQUIRE(engine.get_current_state() == EEngineState::CONSTRUCTED);


			// init with initializer should give the correct id
			auto init = EngineInitializer{ 666 };
			auto engine2 = cEngine(init);
			REQUIRE(engine2.id == 666);
			REQUIRE(engine.get_current_state() == EEngineState::CONSTRUCTED);

		}

		TEST_CASE("cEngine::startup()")
		{
			auto init = EngineInitializer{ 1 };
			auto engine = cEngine(init);

			engine.startup();
			REQUIRE(engine.get_current_state() == EEngineState::STARTUP);
		}

		TEST_CASE("cEngine::run()")
		{
			auto init = EngineInitializer{ 1 };
			auto engine = cEngine(init);

			engine.run();
			REQUIRE(engine.get_current_state() == EEngineState::RUNNING);
		}

		TEST_CASE("cEngine::shutdown()")
		{
			auto init = EngineInitializer{ 1 };
			auto engine = cEngine(init);

			engine.shutdown();
			REQUIRE(engine.get_current_state() == EEngineState::SHUTDOWN);
		}

	#endif // RUN_UNIT_TESTS

#endif // !ENGINE__TST_CC
