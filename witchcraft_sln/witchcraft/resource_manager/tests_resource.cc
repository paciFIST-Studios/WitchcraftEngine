#ifndef TEST_RESOURCE_CC
#define TEST_RESOURCE_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "resource.h"
		
		TEST_CASE("cResource::cResource(args)  When given default arguments, ctor constructs object with those values")
		{
			unsigned int resource_id = 666;
			unsigned int resource_scope = 667;
			std::string file_name = "668";
			RESOURCE_TYPE resource_type = RESOURCE_TYPE::RESOURCE_GRAPHIC;
		
			auto resource = cResource(resource_id, resource_scope, file_name, resource_type);
			REQUIRE(resource.get_resource_id() == resource_id);
			REQUIRE(resource.get_scope() == resource_scope);
			REQUIRE(resource.get_file_name() == file_name);
			REQUIRE(resource.get_resource_type() == resource_type);
		}

		TEST_CASE("cResource::cResource()  Produces correct default object; also, the getter methods work for this data")
		{
			// default object 
			auto resource = &cResource();
			auto file_name = resource->get_file_name();
			auto resource_id = resource->get_resource_id();
			auto resource_type = resource->get_resource_type();
			auto resource_scope = resource->get_scope();

			// file name
			REQUIRE(typeid(file_name) == typeid(std::string));
			REQUIRE(file_name.compare(resource->UNINIT_FILE_NAME));

			// resource id
			REQUIRE(typeid(resource_id) == typeid(resource->UNINIT_RESOURCE_ID));
			REQUIRE(resource_id == resource->UNINIT_RESOURCE_ID);

			// resource type
			REQUIRE(typeid(resource_type) == typeid(resource->UNINIT_RESOURCE_TYPE));
			REQUIRE(resource_type == resource->UNINIT_RESOURCE_TYPE);

			// resource scope
			REQUIRE(typeid(resource_scope) == typeid(resource->UNINIT_RESOURCE_SCOPE));
			REQUIRE(resource_scope == resource->UNINIT_RESOURCE_SCOPE);

			// fn
			REQUIRE_NOTHROW(resource->load());
			REQUIRE_NOTHROW(resource->unload());
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_RESOURCE_CC
