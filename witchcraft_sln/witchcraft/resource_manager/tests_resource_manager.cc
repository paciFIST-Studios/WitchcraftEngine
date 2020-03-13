#ifndef TEST_RESOURCE_MANAGER_CC
#define TEST_RESOURCE_MANAGER_CC

// this header will be whitespace when the testing is switched off
#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS
		
		#include "../catch.hpp"
		#include "resource_manager.h"
		
		#include "../utility/utility.h"
	
		// Tests default initialization
		TEST_CASE("ResourceManager: ctor")
		{
			cResourceManager * rm = nullptr;
			rm = &cResourceManager();

			REQUIRE(rm->get_resource_count() == 0);
			REQUIRE(rm->get_current_scope() == RESOURCE_GLOBAL_SCOPE);
			
			// find by id works before assets are added
			REQUIRE_NOTHROW(rm->find_resource_by_id(0));
			REQUIRE(rm->find_resource_by_id(0) == nullptr);

			// clear all works before assets are added
			REQUIRE_NOTHROW(rm->empty_cache());

			// load_from_xml_file works if a non-file path is specified
			REQUIRE_NOTHROW(rm->load_from_xml_file("fake"));
			REQUIRE(rm->load_from_xml_file("fake") == false);

			// set_current_scope works if used before assets are added
			REQUIRE_NOTHROW(rm->set_current_scope(1));
			REQUIRE(rm->get_current_scope() == 0);
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_RESOURCE_MANAGER_CC

