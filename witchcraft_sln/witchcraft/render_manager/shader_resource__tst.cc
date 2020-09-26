

#ifndef ENGINE_RESOURCE__TST_CC
#define ENGINE_RESOURCE__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "shader_resource.h"
		
		TEST_CASE(" ShaderResource::ctor(args)  When given arguments, ctor constructs object with those values")
		{
			std::string		name	 = "fake_asset";
			std::string		filepath = "";
			EResourceType	type	 = EResourceType::SHADER;
			unsigned int	scope	 = 10;

			auto resource = ShaderResource(name, scope);
			REQUIRE(resource.id > 0);
			REQUIRE(resource.name		== name);
			REQUIRE(resource.filepath	== filepath);
			REQUIRE(resource.type		== type);
			REQUIRE(resource.scope		== scope);
		}

	#endif // RUN_UNIT_TESTS

#endif // !ENGINE_RESOURCE__TST_CC
