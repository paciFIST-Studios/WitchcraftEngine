#ifndef TEST_RENDER_RESOURCE_CC
#define TEST_RENDER_RESOURCE_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "sdl_render_resource.h"
			
		TEST_CASE(" SDLRenderResource::RenderResource()")
		{
			auto rr = &SDLRenderResource();
			REQUIRE(rr != nullptr);

			REQUIRE_NOTHROW(rr->load());
			REQUIRE_NOTHROW(rr->unload());
		}

		TEST_CASE(" SDLRenderResource::SDLRenderResource(unsigned int ID, unsigned int scope, std::string const & fileName)")
		{
			int const scope = 667;
			std::string const name = "buddha";
			std::string const filepath = witchcraft::configuration::buddha_png;
			EResourceType resource_type = EResourceType::IMAGE;

			// the SDLRenderResource sets its own resource type
			auto rr = SDLRenderResource(name, filepath, scope);

			REQUIRE(rr.filepath == filepath);

			REQUIRE(rr.scope == scope);

			REQUIRE(rr.type == resource_type);

			REQUIRE_NOTHROW(rr.load());
			REQUIRE_NOTHROW(rr.unload());
		}
		
		TEST_CASE(" SDLRenderResource::is_loaded()")
		{
			auto rr = SDLRenderResource();

			REQUIRE_NOTHROW(rr.is_loaded());
			REQUIRE(rr.is_loaded() == false);

			REQUIRE_NOTHROW(rr.load());

			// the default file name is "un-init"; intention == non-loadable
			REQUIRE(rr.is_loaded() == false);
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_RENDER_RESOURCE_CC
