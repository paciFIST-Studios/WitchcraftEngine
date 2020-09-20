#ifndef PROJECT_LOADER__TST_CC
#define PROJECT_LOADER__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS
		#include "../catch.hpp"

		#include "project_loader.h"

		class TestProjectLoader : public ProjectLoader
		{
		public:
			TestProjectLoader(MessageBus * mb)
				: ProjectLoader(mb)
			{}

			bool has_ptr_to_message_bus(){
				return message_bus != nullptr; }

			bool has_ptr_to_project_file() {
				return project_file != nullptr; }

		};

		

		TEST_CASE("ProjectLoader::  is constructible  without args")
		{
			auto pl = &ProjectLoader();
			REQUIRE(pl != nullptr);
		}

		TEST_CASE("ProjectLoader:: can get MessageBus ptr")
		{
			auto mb = MessageBus();
			auto pl = TestProjectLoader(&mb);
			REQUIRE(pl.has_ptr_to_message_bus());
		}

		TEST_CASE("ProjectLoader can set project file, when given")
		{
			auto mb = MessageBus();
			auto pl = TestProjectLoader(&mb);
			char const * fake_file = "test file path";

			pl.set_project(fake_file);
			REQUIRE_FALSE(pl.has_ptr_to_project_file());


			char const * real_file = "deleteme.delete";
			std::ofstream outfile(real_file);
			outfile << "delete this file, it's only for testing" << std::endl;
			outfile.close();

			pl.set_project(real_file);
			REQUIRE(pl.has_ptr_to_project_file());

			std::remove(real_file);
		}


	#endif // !RUN_UNIT_TESTS

#endif // !PROJECT_LOADER__TST_CC
