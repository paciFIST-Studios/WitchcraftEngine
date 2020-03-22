#ifndef TEST_TWO_D_LAYER_CC
#define TEST_TWO_D_LAYER_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "2d_layer.h"
		
		TEST_CASE("c2DLayer::ctor")
		{
			c2DLayer layer;

			// ctor can construct
			REQUIRE_NOTHROW(layer = c2DLayer());

			// and default ctor uses un-init values
			REQUIRE(layer._layer_z_order == c2DLayer::UNINIT_Z_ORDER);
			REQUIRE(layer._layer_is_visible == c2DLayer::UNINIT_VISIBILITY);
			REQUIRE(layer._position_x == c2DLayer::UNINIT_COORDINATE_F);
			REQUIRE(layer._position_y == c2DLayer::UNINIT_COORDINATE_F);
			REQUIRE(layer._name == "un-init");
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_TWO_D_LAYER_CC
