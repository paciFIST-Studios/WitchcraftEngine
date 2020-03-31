#ifndef RESOURCE__TST_CC
#define RESOURCE__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "resource.h"
		
		TEST_CASE(" qResource::qResource(args)  When given default arguments, ctor constructs object with those values")
		{
			unsigned int resource_id = 666;
			unsigned int resource_scope = 667;
			std::string file_name = "668";
			RESOURCE_TYPE resource_type = RESOURCE_TYPE::RESOURCE_GRAPHIC;
		
			auto resource = qResource(resource_id, resource_scope, file_name, resource_type);
			REQUIRE(resource.get_resource_id() == resource_id);
			REQUIRE(resource.get_scope_id() == resource_scope);
			REQUIRE(resource.get_file_name() == file_name);
			REQUIRE(resource.get_resource_type() == resource_type);
		}

		TEST_CASE(" qResource::qResource()  Produces correct default object; also, the getter methods work for this data")
		{
			// default object 
			auto resource = &qResource();
			auto file_name = resource->get_file_name();
			auto resource_id = resource->get_resource_id();
			auto resource_type = resource->get_resource_type();
			auto resource_scope = resource->get_scope_id();

			// file name
			REQUIRE(typeid(file_name) == typeid(std::string));
			REQUIRE(file_name.compare(std::string(uninit::CSTRING)));

			// resource id
			REQUIRE(typeid(resource_id) == typeid(unsigned int));
			REQUIRE(resource_id == uninit::UINT);

			// resource type
			REQUIRE(typeid(resource_type) == typeid(RESOURCE_TYPE));
			REQUIRE(resource_type == RESOURCE_TYPE::RESOURCE_NULL);

			// resource scope
			REQUIRE(typeid(resource_scope) == typeid(unsigned int));
			REQUIRE(resource_scope == uninit::UINT);

			// fn
			REQUIRE_NOTHROW(resource->load());
			REQUIRE_NOTHROW(resource->unload());
		}

	#endif // RUN_UNIT_TESTS

#endif // !RESOURCE__TST_CC
