#ifndef LAYER_TWO_D__TST_CC
#define LAYER_TWO_D__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "layer_2d.h"
		
		TEST_CASE(" Layer2D::ctor")
		{
			Layer2D layer;

			// ctor can construct
			REQUIRE_NOTHROW(layer = Layer2D());

			// and default ctor uses un-init values
			REQUIRE(layer._layer_z_order == uninit::UINT);
			REQUIRE(layer._layer_is_visible == uninit::BOOL);
			REQUIRE(layer.position_x_ == uninit::FLOAT);
			REQUIRE(layer.position_y_ == uninit::FLOAT);
			REQUIRE(layer._name == std::string(uninit::CSTRING));
		}

	#endif // RUN_UNIT_TESTS

#endif // !LAYER_TWO_D__TST_CC
