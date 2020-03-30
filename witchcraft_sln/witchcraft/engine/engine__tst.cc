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

			// init with initializer should give the correct id
			auto einit = EngineInitializer{ 666 };
			auto engine2 = cEngine(einit);
			REQUIRE(engine2.id == 666);

		}

	#endif // RUN_UNIT_TESTS

#endif // !ENGINE__TST_CC
