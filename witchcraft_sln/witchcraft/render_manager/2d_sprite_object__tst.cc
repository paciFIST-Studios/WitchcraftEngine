#ifndef TWO_D_SPRITE_OBJECT__TST_CC
#define TWO_D_SPRITE_OBJECT__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "2d_sprite_object.h"

		TEST_CASE("c2DSpriteObject::ctor")
		{
			auto so = c2DSpriteObject();
	
			auto sprite_dimension = so.get_sprite_sheet_dimensions();
			REQUIRE(std::get<0>(sprite_dimension) == uninit::UINT);
			REQUIRE(std::get<1>(sprite_dimension) == uninit::UINT);

			auto frame_dimension = so.get_frame_dimensions();
			REQUIRE(std::get<0>(frame_dimension) == uninit::UINT);
			REQUIRE(std::get<1>(frame_dimension) == uninit::UINT);

			auto frame_position = so.get_frame_position();
			REQUIRE(std::get<0>(frame_position) == uninit::UINT);
			REQUIRE(std::get<1>(frame_position) == uninit::UINT);

			REQUIRE(so.get_current_animation() == nullptr);

			REQUIRE(so.get_current_frame_timing() == uninit::FLOAT);
			REQUIRE(so.is_animation_playing() == uninit::BOOL);

			REQUIRE_NOTHROW(so.update());
			REQUIRE_NOTHROW(so.play());
			REQUIRE_NOTHROW(so.stop());
			REQUIRE_NOTHROW(so.set_frame_rect(0));
		}

		TEST_CASE("c2DSpriteObject::ctor(initializer object)")
		{
			c2DSpriteObjectInitializer soi;
			soi.sprite_sheet_width = 32;
			soi.sprite_sheet_height = 32;
			soi.frame_width = 16;
			soi.frame_height = 16;
			soi.frame_position_x = 16;
			soi.frame_position_y = 16;

			auto so = c2DSpriteObject(soi);
			
			auto sprite_dimension = so.get_sprite_sheet_dimensions();
			REQUIRE(std::get<0>(sprite_dimension) == soi.sprite_sheet_width);
			REQUIRE(std::get<1>(sprite_dimension) == soi.sprite_sheet_height);

			auto frame_dimension = so.get_frame_dimensions();
			REQUIRE(std::get<0>(frame_dimension) == soi.frame_width);
			REQUIRE(std::get<1>(frame_dimension) == soi.frame_height);

			auto frame_position = so.get_frame_position();
			REQUIRE(std::get<0>(frame_position) == soi.frame_position_x);
			REQUIRE(std::get<1>(frame_position) == soi.frame_position_y);
		}

		TEST_CASE("c2DSpriteObject::play")
		{
			auto so = c2DSpriteObject();
			so.stop();
			REQUIRE(so.is_animation_playing() == false);
			so.play();
			REQUIRE(so.is_animation_playing() == true);
		}

		TEST_CASE("c2DSpriteObject::stop")
		{
			auto so = c2DSpriteObject();
			so.play();
			REQUIRE(so.is_animation_playing() == true);
			so.stop();
			REQUIRE(so.is_animation_playing() == false);
		}


	#endif // RUN_UNIT_TESTS

#endif // !TWO_D_SPRITE_OBJECT__TST_CC
