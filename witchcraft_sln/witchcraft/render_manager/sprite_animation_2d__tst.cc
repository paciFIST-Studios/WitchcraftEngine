#ifndef SPRITE_ANIMATION_TWO_D__TST_CC
#define SPRITE_ANIMATION_TWO_D__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../uninitialized_values.h"
		#include "sprite_animation_2d.h"

		TEST_CASE(" Animation2D::ctor"  )
		{
			auto anim = Animation2D();
			REQUIRE(anim.get_name() == uninit::CSTRING);
			REQUIRE(anim.get_atlas_name() == uninit::CSTRING);
			REQUIRE(anim.get_atlas_uuid() == uninit::UINT);
			REQUIRE(anim.get_ms_per_frame() == uninit::UINT);

			// we can't tell that it's looping the same zero,
			// but we do know it should report a zero each time
			REQUIRE(anim.get_next_index() == uninit::UINT);
			REQUIRE(anim.get_next_index() == uninit::UINT);
			REQUIRE(anim.get_next_index() == uninit::UINT);
		}
	
		TEST_CASE(" Animation2D::Animation2D(name, sequence, timing)")
		{
			std::string anim_name = "walk";
			std::string atlas_name = "sprite_atlas_player_1";
			unsigned int atlas_uuid = 666;
			unsigned int ms_per_frame = 500;
			std::vector<unsigned int> anim_sequence = { 1, 2, 3 };


			auto anim = Animation2D(
				  anim_name
				, atlas_name
				, atlas_uuid
				, ms_per_frame
				, anim_sequence
			);

			REQUIRE(anim.get_name() == anim_name);
			REQUIRE(anim.get_atlas_name() == atlas_name);
			REQUIRE(anim.get_atlas_uuid() == atlas_uuid);
			REQUIRE(anim.get_ms_per_frame() == ms_per_frame);

			REQUIRE(anim.get_next_index() == 1);
			REQUIRE(anim.get_next_index() == 2);
			REQUIRE(anim.get_next_index() == 3);
			REQUIRE(anim.get_next_index() == 1);
			REQUIRE(anim.get_next_index() == 2);
			REQUIRE(anim.get_next_index() == 3);
			REQUIRE(anim.get_next_index() == 1);
		}

	#endif // RUN_UNIT_TESTS

#endif // !SPRITE_ANIMATION_TWO_D__TST_CC
