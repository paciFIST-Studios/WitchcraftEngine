#ifndef TEST_2D_SCENE_MANAGER_CC
#define TEST_2D_SCENE_MANAGER_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "scene_manager_2d.h"
		
		TEST_CASE(" SceneManager2D::SceneManager2D()")
		{
			REQUIRE_NOTHROW(SceneManager2D());
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_2D_SCENE_MANAGER_CC
