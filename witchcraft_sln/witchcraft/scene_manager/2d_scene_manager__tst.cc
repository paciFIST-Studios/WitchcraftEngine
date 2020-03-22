#ifndef TEST_2D_SCENE_MANAGER_CC
#define TEST_2D_SCENE_MANAGER_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "2d_scene_manager.h"
		
		TEST_CASE("c2DSceneManager::c2DSceneManager()")
		{
			REQUIRE_NOTHROW(c2DSceneManager());
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_2D_SCENE_MANAGER_CC
