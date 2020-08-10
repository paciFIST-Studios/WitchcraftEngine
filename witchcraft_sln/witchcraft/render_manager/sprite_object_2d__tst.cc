#ifndef SPRITE_OBJECT_TWO_D__TST_CC
#define SPRITE_OBJECT_TWO_D__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "sprite_object_2d.h"

		TEST_CASE(" c2DSpriteObject::ctor")
		{
			// Tests default init values, and calls all methods

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

			// getters
			REQUIRE_NOTHROW(so.get_sprite_sheet_dimensions());
			REQUIRE_NOTHROW(so.get_frame_dimensions());
			REQUIRE_NOTHROW(so.get_frame_position());
			REQUIRE_NOTHROW(so.get_current_animation());
			REQUIRE_NOTHROW(so.get_current_frame_timing());
			REQUIRE_NOTHROW(so.is_animation_playing());

			// actual methods
			REQUIRE_NOTHROW(so.update());
			REQUIRE_NOTHROW(so.play());
			REQUIRE_NOTHROW(so.stop());
			REQUIRE_NOTHROW(so.set_frame_rect(0));
		}

		TEST_CASE(" c2DSpriteObject::ctor(initializer object)")
		{
			// initializer
			c2DSpriteObjectInitializer soi;
			soi.sprite_sheet_width = 32;
			soi.sprite_sheet_height = 32;
			soi.frame_width = 16;
			soi.frame_height = 16;
			soi.frame_position_x = 16;
			soi.frame_position_y = 16;

			// specific animation
			std::string anim_name = "test";
			std::string atlas_name = "test_atlas";
			unsigned int atlas_uuid = 666;
			unsigned int anim_speed = 10;
			std::vector<unsigned int> anim_sequence = { 1, 3, 5, 7, 2, 4, 6, 8 };

			auto test_anim = Animation2D(
				  anim_name
				, atlas_name
				, atlas_uuid
				, anim_speed
				, anim_sequence
			);

			soi.animations.push_back(test_anim);

			// initialized object
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

			REQUIRE_NOTHROW(so.set_animation(anim_name));
			auto ca = so.get_current_animation();
			REQUIRE(ca != nullptr);
			REQUIRE(ca->get_name() == anim_name);
			REQUIRE(ca->get_atlas_name() == atlas_name);
			REQUIRE(ca->get_atlas_uuid() == atlas_uuid);
			REQUIRE(ca->get_ms_per_frame() == anim_speed);
			REQUIRE(ca->get_next_index() == 1);
			REQUIRE(ca->get_next_index() == 3);
			REQUIRE(ca->get_next_index() == 5);
			REQUIRE(ca->get_next_index() == 7);
			REQUIRE(ca->get_next_index() == 2);
			REQUIRE(ca->get_next_index() == 4);
			REQUIRE(ca->get_next_index() == 6);
			REQUIRE(ca->get_next_index() == 8);
			REQUIRE(ca->get_next_index() == 1); // all sequences perma-loop
		}

		TEST_CASE(" c2DSpriteObject::play")
		{
			auto so = c2DSpriteObject();
			so.stop();
			REQUIRE(so.is_animation_playing() == false);
			so.play();
			REQUIRE(so.is_animation_playing() == true);
		}

		TEST_CASE(" c2DSpriteObject::stop")
		{
			auto so = c2DSpriteObject();
			so.play();
			REQUIRE(so.is_animation_playing() == true);
			so.stop();
			REQUIRE(so.is_animation_playing() == false);
		}

	#endif // RUN_UNIT_TESTS

#endif // !SPRITE_OBJECT_TWO_D__TST_CC
