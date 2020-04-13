#ifndef RENDER_MANAGER_TWO_D__TST_CC
#define RENDER_MANAGER_TWO_D__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include <string>

		#include "render_manager_2d.h"

		TEST_CASE(" RenderManager2D")
		{
			
			//  draw any loaded graphical item
			// drawn item appears at any valid xy position
		}

		class TestRenderManager2D : public RenderManager2D
		{
		public:
		};
		
		TEST_CASE(" RenderManager2D:: ")
		{
		}

	#endif // RUN_UNIT_TESTS

#endif // !RENDER_MANAGER_TWO_D__TST_CC


