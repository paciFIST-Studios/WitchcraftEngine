#ifndef TEST_TWO_D_LAYER_CC
#define TEST_TWO_D_LAYER_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "2d_layer.h"
		
		TEST_CASE("c2DLayer::c2DLayer()")
		{
			REQUIRE_NOTHROW(c2DLayer());
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_TWO_D_LAYER_CC
