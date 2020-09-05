#ifndef RENDER_MANAGER__TST_CC
#define RENDER_MANAGER__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include <string>

		#include "render_manager.h"

		TEST_CASE(" RenderManager")
		{			
			// draw any loaded graphical item
			// drawn item appears at any valid xy position
		}

		class TestRenderManager2D : public RenderManager
		{
		public:
		};
		
		TEST_CASE(" RenderManager:: ")
		{
		}

	#endif // RUN_UNIT_TESTS

#endif // !RENDER_MANAGER__TST_CC


