#ifndef ENGINE_OBJECT__TST_CC
#define ENGINE_OBJECT__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../engine/engine_object.h"

		TEST_CASE(" qEngineObject::ctor   instantiates object when used")
		{
			REQUIRE_NOTHROW(qEngineObject());
			auto eo = &qEngineObject();
			REQUIRE(eo != nullptr);
		}

		TEST_CASE(" qEngineObject  is instantiated with a higher ID each time")
		{
			auto a = qEngineObject();
			auto b = qEngineObject();
			auto c = qEngineObject();

			REQUIRE(a.id < b.id);
			REQUIRE(b.id < c.id);
		}

	#endif // RUN_UNIT_TESTS

#endif // !ENGINE_OBJECT__TST_CC


