#ifndef TEST_RENDER_RESOURCE_CC
#define TEST_RENDER_RESOURCE_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "render_resource.h"
			
		TEST_CASE("cRenderResource::RenderResource()")
		{
			auto rr = &cRenderResource();
			REQUIRE(rr != nullptr);
			REQUIRE_NOTHROW(rr->get_file_name());
			REQUIRE_NOTHROW(rr->get_resource_id());
			REQUIRE_NOTHROW(rr->get_resource_type());
			REQUIRE_NOTHROW(rr->get_scope());
			REQUIRE_NOTHROW(rr->load());
			REQUIRE_NOTHROW(rr->unload());
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_RENDER_RESOURCE_CC
