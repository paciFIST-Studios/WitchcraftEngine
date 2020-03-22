#ifndef TEST_SCENE_LISTENER_CC
#define TEST_SCENE_LISTENER_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "scene_listener.h"
		
		TEST_CASE("cSceneListener::cSceneListener()")
		{
			REQUIRE_NOTHROW(cSceneListener());
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_SCENE_LISTENER_CC
