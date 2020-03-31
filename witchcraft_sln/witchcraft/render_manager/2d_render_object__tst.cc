#ifndef TWO_D_RENDER_OBJECT__TST_CC
#define TWO_D_RENDER_OBJECT__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../uninitialized_values.h"
		#include "2d_render_object.h"

		TEST_CASE(" c2DRenderObject::ctor"  )
		{
			auto ro = c2DRenderObject();

			REQUIRE(ro.is_visible() == uninit::BOOL);

			auto pos = ro.get_position();
			REQUIRE(typeid(pos) == typeid(POSITION_TUPLE));
			REQUIRE(std::get<0>(pos) == uninit::FLOAT);
			REQUIRE(std::get<1>(pos) == uninit::FLOAT);

		}

		TEST_CASE(" c2DRenderObject:: get/set position()")
		{
			int const XPOS = 10;
			int const YPOS = 20;

			auto ro = c2DRenderObject();

			// get/set should not throw exceptions
			REQUIRE_NOTHROW(ro.get_position());

			REQUIRE_NOTHROW(ro.set_position(XPOS, YPOS));
			auto pos = ro.get_position();

			// tuple should contain correct values
			REQUIRE(std::get<0>(pos) == XPOS);
			REQUIRE(std::get<1>(pos) == YPOS);

			// X should be a float, and Y should be a float
			REQUIRE(typeid(std::get<0>(pos)) == typeid(float));
			REQUIRE(typeid(std::get<0>(pos)) == typeid(std::get<1>(pos)));
		}

		TEST_CASE(" c2DRenderObject:: get/set is_visible")
		{
			auto ro = c2DRenderObject();
			REQUIRE_NOTHROW(ro.set_is_visible(true));
			REQUIRE(ro.is_visible());
			ro.set_is_visible(false);
			REQUIRE(ro.is_visible() == false);
		}



	#endif // RUN_UNIT_TESTS

#endif // !TWO_D_RENDER_OBJECT__TST_CC
