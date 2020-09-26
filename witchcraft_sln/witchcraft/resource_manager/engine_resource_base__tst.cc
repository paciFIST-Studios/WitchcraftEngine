#ifndef ENGINE_RESOURCE__TST_CC
#define ENGINE_RESOURCE__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "engine_resource_base.h"
		
		TEST_CASE(" EngineResourceBase::ctor(args)  When given arguments, ctor constructs object with those values")
		{
			std::string		name	 = "fake_asset";
			std::string		filepath = "asset/game/testing/fake_asset.png";
			EResourceType	type	 = EResourceType::IMAGE;
			unsigned int	scope	 = 10;

			auto resource = EngineResourceBase(name, filepath, type, scope);
			REQUIRE(resource.id > 0);
			REQUIRE(resource.name		== name);
			REQUIRE(resource.filepath	== filepath);
			REQUIRE(resource.type		== type);
			REQUIRE(resource.scope		== scope);
		}

		TEST_CASE(" EngineResourceBase::EngineResourceBase()  Produces correct default object")
		{
			// default object 
			auto r = &EngineResourceBase();
			REQUIRE(r != nullptr);

			// name
			REQUIRE(typeid(r->name) == typeid(std::string));
			REQUIRE(r->name.compare(std::string("uninit")));

			// filepath
			REQUIRE(typeid(r->filepath) == typeid(std::string));
			REQUIRE(r->filepath.compare(std::string("uninit")));

			// type
			REQUIRE(typeid(r->type) == typeid(EResourceType));
			REQUIRE(r->type == EResourceType::UNINITIALIZED);

			REQUIRE(typeid(r->scope) == typeid(int));
			REQUIRE(r->scope == -1);

			// fn
			REQUIRE_NOTHROW(r->load());
			REQUIRE_NOTHROW(r->unload());
		}

	#endif // RUN_UNIT_TESTS

#endif // !ENGINE_RESOURCE__TST_CC
