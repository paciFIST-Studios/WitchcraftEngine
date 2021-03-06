#ifndef TEST_SCENE_OBJECT_CC
#define TEST_SCENE_OBJECT_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "scene_object.h"
		
		TEST_CASE(" qSceneObject::qSceneObject()")
		{
			REQUIRE_NOTHROW(qSceneObject());
			auto ptr = &qSceneObject();
			REQUIRE(ptr != nullptr);
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_SCENE_OBJECT_CC
