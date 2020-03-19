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

		TEST_CASE("cRenderResource::cRenderResource(unsigned int ID, unsigned int scope, std::string const & fileName)")
		{
			unsigned int const resource_id = 666;
			unsigned int const resource_scope = 667;
			std::string const file_name = "buddha.png";
			RESOURCE_TYPE resource_type = RESOURCE_TYPE::RESOURCE_GRAPHIC;

			// the cRenderResource sets its own resource type
			auto rr = cRenderResource(resource_id, resource_scope, file_name);

			REQUIRE_NOTHROW(rr.get_file_name());
			REQUIRE(rr.get_file_name() == file_name);

			REQUIRE_NOTHROW(rr.get_resource_id());
			REQUIRE(rr.get_resource_id() == resource_id);

			REQUIRE_NOTHROW(rr.get_scope());
			REQUIRE(rr.get_scope() == resource_scope);

			REQUIRE_NOTHROW(rr.get_resource_type());
			REQUIRE(rr.get_resource_type() == resource_type);

			REQUIRE_NOTHROW(rr.load());
			REQUIRE_NOTHROW(rr.unload());
		}
		
		TEST_CASE("cRenderResource::is_loaded()")
		{
			auto rr = cRenderResource();

			REQUIRE_NOTHROW(rr.is_loaded());
			REQUIRE(rr.is_loaded() == false);

			REQUIRE_NOTHROW(rr.load());

			// the default file name is "un-init"; intention == non-loadable
			REQUIRE(rr.is_loaded() == false);
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_RENDER_RESOURCE_CC
