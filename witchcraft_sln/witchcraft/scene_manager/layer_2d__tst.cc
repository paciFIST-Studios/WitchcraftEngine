#ifndef LAYER_TWO_D__TST_CC
#define LAYER_TWO_D__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "layer_2d.h"
		
		TEST_CASE(" Layer2D::ctor::uninit")
		{
			Layer2D layer = Layer2D();

			// and default ctor uses un-init values
			REQUIRE(layer.get_z_order() == uninit::UINT);
			REQUIRE(layer.get_is_visible() == uninit::BOOL);

			auto tuple = layer.get_offset();
			REQUIRE(std::get<0>(tuple) == uninit::FLOAT);
			REQUIRE(std::get<1>(tuple) == uninit::FLOAT);
			REQUIRE(layer.get_name() == std::string(uninit::CSTRING));
		}

		TEST_CASE(" Layer2D::ctor")
		{
			REQUIRE_NOTHROW(Layer2D());

			unsigned int const test_id = 666;
			bool const test_visibility = true;
			std::string const test_name = "background_1";
			float const test_x = 10;
			float const test_y = 12;
			unsigned int const test_z_order = 1;

			Layer2DInitializer init;
			init.is_visible = test_visibility;
			init.name = test_name;
			init.x = test_x;
			init.y = test_y;
			init.zOrder = test_z_order;

			auto layer = Layer2D(init);

			REQUIRE(layer.get_is_visible() == test_visibility);
			REQUIRE(layer.get_name() == test_name);

			auto tuple = layer.get_offset();
			REQUIRE(std::get<0>(tuple) == test_x);
			REQUIRE(std::get<1>(tuple) == test_y);

			REQUIRE(layer.get_z_order() == test_z_order);
		}

		TEST_CASE(" Layer2D:: is_visible")
		{
			bool test_value = false;
			auto layer = Layer2D();

			layer.set_is_visible(test_value);
			REQUIRE(layer.get_is_visible() == test_value);

			layer.set_is_visible(!test_value);
			REQUIRE(layer.get_is_visible() == !test_value);
		}

		TEST_CASE(" Layer2D:: z_order")
		{
			unsigned int const test_value = 666;
			auto layer = Layer2D();
			
			REQUIRE(layer.get_z_order() == uninit::UINT);
			layer.set_z_order(test_value);
			REQUIRE(layer.get_z_order() == test_value);
			layer.set_z_order(test_value + 1);
			REQUIRE(layer.get_z_order() == test_value + 1);
		}

		TEST_CASE(" Layer2D:: offset")
		{
			float const x = 1;
			float const y = 2;

			auto layer = Layer2D();

			layer.set_offset(x, y);
			auto tuple = layer.get_offset();
			REQUIRE(std::get<0>(tuple) == x);
			REQUIRE(std::get<1>(tuple) == y);

			layer.set_offset(x+1, y+1);
			auto tuple2 = layer.get_offset();
			REQUIRE(std::get<0>(tuple2) == x+1);
			REQUIRE(std::get<1>(tuple2) == y+1);
		}

	#endif // RUN_UNIT_TESTS

#endif // !LAYER_TWO_D__TST_CC
