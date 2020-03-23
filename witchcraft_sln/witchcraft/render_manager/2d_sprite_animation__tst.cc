#ifndef TWO_D_SPRITE_ANIMATION__TST_CC
#define TWO_D_SPRITE_ANIMATION__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../uninitialized_values.h"
		#include "2d_sprite_animation.h"

		TEST_CASE("c2DSpriteAnimation::ctor"  )
		{
			auto anim = c2DSpriteAnimation();
			REQUIRE(anim.get_name() == uninit::CSTRING);
			REQUIRE(anim.get_frame_advance_timing() == uninit::UINT);

			// we can't tell that it's looping the same zero,
			// but we do know it should report a zero each time
			REQUIRE(anim.get_next_frame() == uninit::UINT);
			REQUIRE(anim.get_next_frame() == uninit::UINT);
			REQUIRE(anim.get_next_frame() == uninit::UINT);
		}
	
		TEST_CASE("c2DSpriteAnimation::c2DSpriteAnimation(name, sequence, timing)")
		{
			std::string anim_name = "walk";
			std::vector<unsigned int> anim_sequence = { 1, 2, 3 };
			unsigned int anim_timing = 500;

			auto anim = c2DSpriteAnimation(anim_name, anim_sequence, anim_timing);
			REQUIRE(anim.get_name() == anim_name);
			REQUIRE(anim.get_frame_advance_timing() == anim_timing);

			REQUIRE(anim.get_next_frame() == 1);
			REQUIRE(anim.get_next_frame() == 2);
			REQUIRE(anim.get_next_frame() == 3);
			REQUIRE(anim.get_next_frame() == 1);
			REQUIRE(anim.get_next_frame() == 2);
			REQUIRE(anim.get_next_frame() == 3);
			REQUIRE(anim.get_next_frame() == 1);
		}



	#endif // RUN_UNIT_TESTS

#endif // !TWO_D_SPRITE_ANIMATION__TST_CC
