#ifndef SPRITE_ATLAS_RESOURCE__TST_CC
#define SPRITE_ATLAS_RESOURCE__TST_CC

	
	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../uninitialized_values.h"

		TEST_CASE("")
		{
			REQUIRE(true);
		}

	#endif // !RUN_UNIT_TESTS

#endif // !SPRITE_ATLAS_RESOURCE__TST_CC