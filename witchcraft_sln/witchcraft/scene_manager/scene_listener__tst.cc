#ifndef TEST_SCENE_LISTENER_CC
#define TEST_SCENE_LISTENER_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "scene_listener.h"
	
		class TestSceneListener : public cSceneListener
		{		
			void event(c2DSceneManager const & manager, void * callback_data) const override
			{
			}
		};

		TEST_CASE(" TestSceneListener::ctor")
		{
			// We should be able to override an ABC w/o causing problems
			REQUIRE_NOTHROW(TestSceneListener());
		}


		// NOTES: This is an abstract base class (It's meant to be), so we can't
		// really run a test on it, because: 
		//		if it remains an abstract base class, it will throw a compiler error
		//		if you ever try to instantiate it.
		//
		//		as a result, if we write the code that will test instantiation, then
		//		we are also writing code that cannot compile.
		//		it would work as a test, ish, but it would also prevent the project from	
		//		being compiled, and that would cause the project to fail.  
		//
		//		So for right now, to run the test, you manually comment-in the 
		//			#define FAIL_COMPILATION
		//		line, and then you try to compile.  If the compilation fails, then the 
		//		test works but also you'll have to comment out the #define again, 
		//		before you can actually compile the project.
		//
		// the result is, this kind of has to be a manual test. finding an automated mechanism
		// is a low priority

		//#define FAIL_COMPILATION

		#ifdef FAIL_COMPILATION
		TEST_CASE(" cSceneListener::cSceneListener() compilation fails for abstract base class")
		{
			REQUIRE_NOTHROW(cSceneListener());
		}
		#endif // !FAIL_COMPILATION

	#endif // RUN_UNIT_TESTS

#endif // !TEST_SCENE_LISTENER_CC
