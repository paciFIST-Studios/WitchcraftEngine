#ifndef RENDER_MANAGER_TWO_D__TST_CC
#define RENDER_MANAGER_TWO_D__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "render_manager_2d.h"

		TEST_CASE(" RenderManager2D")
		{
			
			//  draw any loaded graphical item
			// drawn item appears at any valid xy position
		}

		TEST_CASE(" RenderManager2D:: set_scene_layers_callback")
		{
			auto rm = RenderManager2D();
			auto sm = SceneManager2D();

			auto scene_cb = sm.get_scene_layers_callback();
			rm.set_scene_layers_callback(scene_cb);



		}

	#endif // RUN_UNIT_TESTS

#endif // !RENDER_MANAGER_TWO_D__TST_CC


