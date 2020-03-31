#ifndef ENGINE_OBJECT__TST_CC
#define ENGINE_OBJECT__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../engine/engine_object.h"

		TEST_CASE("qEngineObject::ctor")
		{
			auto eo = qEngineObject();
			REQUIRE(eo.id == 0);

			qEngineObject eo2(2);
			REQUIRE(eo2.id == 2);
		}

	#endif // RUN_UNIT_TESTS

#endif // !ENGINE_OBJECT__TST_CC


