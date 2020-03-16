#ifndef TEST_RESOURCE_MANAGER_CC
#define TEST_RESOURCE_MANAGER_CC

// this header will be whitespace when the testing is switched off
#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS
		
		#include "../catch.hpp"
		#include "resource_manager.h"
		
		#include "../utility/utility.h"
	
		std::string const bird = "asset/birds.asset";
		std::string const buddha = "asset/buddha.asset";
		std::string const person = "asset/person.asset";

		// Tests default initialization
		TEST_CASE("ResourceManager: ctor")
		{
			auto rm = cResourceManager();

			// Starts w/ no resources
			REQUIRE(rm.get_resource_count() == 0);

			// starts w/ global scope
			REQUIRE(rm.get_current_scope() == RESOURCE_GLOBAL_SCOPE);
			
			// find by id does not throw error, even before anthing is loaded
			REQUIRE_NOTHROW(rm.find_resource_by_id(0));

			// find by id returns nullptr, if no match is found
			REQUIRE(rm.find_resource_by_id(0) == nullptr);

			// empty_cache does not throw, even before anything is loaded
			REQUIRE_NOTHROW(rm.empty_cache());

			// load_from_xml_file does not throw, if file not found
			REQUIRE_NOTHROW(rm.load_from_xml_file("fake"));

			// load_from_xml_file returns false if file not found 
			REQUIRE(rm.load_from_xml_file("fake") == false);

			// set_current_scope does not throw, even before anything is loaded
			REQUIRE_NOTHROW(rm.set_current_scope(1));
			
			// set_current_scope returns false, if no asset has been loaded
			REQUIRE(rm.set_current_scope(1) == false);
		}

		TEST_CASE("ResourceManager + RenderManager Load rendering assets")
		{
			auto rm = cResourceManager();
			// includes some testing files
			rm.create_config_files();

			// default scope
			REQUIRE(rm.get_current_scope() == RESOURCE_GLOBAL_SCOPE);

			REQUIRE(rm.get_resource_count() == 0);

			// Load returns true on success
			REQUIRE(rm.load_from_xml_file(buddha) == true);
			
			REQUIRE(rm.get_resource_count() == 1);

			// loading a resource does not change scope
			REQUIRE(rm.get_current_scope() == RESOURCE_GLOBAL_SCOPE);

			rm.load_from_xml_file(bird);
			REQUIRE(rm.get_resource_count() == 2);
			// Loading a file with a different scenescope does not change current scope
			REQUIRE(rm.get_current_scope() == 0);

			rm.load_from_xml_file(person);
			// loading assets will always increase asset count
			REQUIRE(rm.get_resource_count() == 3);

			// loading the same file again (as defined by resource_id) returns false
			REQUIRE(rm.load_from_xml_file(person) == false);
			REQUIRE(rm.load_from_xml_file(bird) == false);
		}

		TEST_CASE("ResourceManager::find_resource_by_id(unsigned int UID)")
		{
			auto rm = cResourceManager();
			rm.create_config_files();

			// does not throw, even if nothing has been loaded yet
			REQUIRE_NOTHROW(rm.find_resource_by_id(MAXUINT32));

			auto resource = rm.find_resource_by_id(MAXUINT32);
			// returns nullptr if no resource found
			REQUIRE(resource == nullptr);

			// actual load
			rm.load_from_xml_file(bird);

			resource = rm.find_resource_by_id(1);
			// Returns active ptr, if object is loaded
			REQUIRE(resource != nullptr);
			// ID actually matches our request
			REQUIRE(resource->_resource_id == 1);
			// returned resource exists in its own scope, as defined by file
			REQUIRE(resource->_scope != RESOURCE_GLOBAL_SCOPE);
		}

		TEST_CASE("ResourceManager:: get_current_scope() / set_current_scope(unsigned int)")
		{
			auto rm = cResourceManager();

			// default scope is global
			REQUIRE(rm.get_current_scope() == RESOURCE_GLOBAL_SCOPE);
	
			// changing scope before loading any resources fails
			REQUIRE(rm.set_current_scope(1) == false);

			rm.load_from_xml_file(bird);

			// after any resource is loaded, this should always return true			
			REQUIRE(rm.set_current_scope(1) == true);
			REQUIRE(rm.set_current_scope(2) == true);

			// after loading something, we can change scope to a scope w/ no assets
			REQUIRE(rm.set_current_scope(100) == true);
		}

		TEST_CASE("ResourceManager::empty_cache()")
		{
			auto rm = cResourceManager();
			rm.create_config_files();

			// Does not throw, even if you call before anything has been loaded
			REQUIRE_NOTHROW(rm.empty_cache());
			REQUIRE(rm.get_resource_count() == 0);

			rm.load_from_xml_file(buddha);
			rm.load_from_xml_file(bird);
			rm.load_from_xml_file(person);

			REQUIRE(rm.get_current_scope() == 0);
			REQUIRE(rm.get_resource_count() == 3);

			rm.empty_cache();

			REQUIRE(rm.get_current_scope() == 0);
			// Empties the entire cache, even other scenescopes
			REQUIRE(rm.get_resource_count() == 0);

			// load moar
			rm.load_from_xml_file(buddha);
			rm.load_from_xml_file(bird);
			rm.load_from_xml_file(person);

			rm.set_current_scope(2);
			REQUIRE(rm.get_current_scope() == 2);

			rm.empty_cache();
			// emptying cache also sets back to global scope
			REQUIRE(rm.get_current_scope() == RESOURCE_GLOBAL_SCOPE);
		}


	#endif // RUN_UNIT_TESTS

#endif // !TEST_RESOURCE_MANAGER_CC

