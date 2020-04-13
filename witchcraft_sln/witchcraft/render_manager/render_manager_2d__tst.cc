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
			witchcraft::callback_types::SceneLayersCallbackType2 get_stored_cb() const
			{
				return cb_get_layers;
			}

			bool use_stored_cb()
			{
				if (cb_get_layers == nullptr)
				{
					return false;
				}

				auto ret = cb_get_layers();
				auto ret_ptr = &ret;
				if (ret_ptr == nullptr)
				{
					return false;
				}

				return true;
			}
		};
		
		TEST_CASE(" RenderManager2D:: set_scene_layers_callback integration and use")
		{
			auto rm = TestRenderManager2D();
			auto sm = SceneManager2D();

			for (int i = 0; i < 5; i++)
			{
				sm.add_layer("test_" + std::to_string(i));
			}

			auto scene_cb = sm.get_scene_layers_callback();
			rm.set_scene_layers_callback(scene_cb);

			auto cb = rm.get_stored_cb();
			REQUIRE(cb != nullptr);

			// returns true if the cb was called, and then we got the return value
			REQUIRE(rm.use_stored_cb());
		}

	#endif // RUN_UNIT_TESTS

#endif // !RENDER_MANAGER_TWO_D__TST_CC


