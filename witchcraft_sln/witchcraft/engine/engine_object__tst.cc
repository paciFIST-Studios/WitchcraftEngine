#ifndef ENGINE_OBJECT__TST_CC
#define ENGINE_OBJECT__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../engine/engine_object.h"

		TEST_CASE(" EngineObjectBase::ctor   instantiates object when used")
		{
			REQUIRE_NOTHROW(EngineObjectBase());
			auto eo = &EngineObjectBase();
			REQUIRE(eo != nullptr);
		}

		TEST_CASE(" EngineObjectBase  is instantiated with a higher ID each time")
		{
			auto a = EngineObjectBase();
			auto b = EngineObjectBase();
			auto c = EngineObjectBase();

			REQUIRE(a.id < b.id);
			REQUIRE(b.id < c.id);
		}

		TEST_CASE(" EngineObjectBase  is instantiated higher that offset")
		{
			auto eo = EngineObjectBase();
			REQUIRE(eo.id > witchcraft::engine::engine_id_offset);
		}

	#endif // RUN_UNIT_TESTS

#endif // !ENGINE_OBJECT__TST_CC


