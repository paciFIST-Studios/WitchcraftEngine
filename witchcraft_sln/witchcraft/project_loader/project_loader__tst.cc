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
			TestProjectLoader(std::string path)
				: ProjectLoader(path)
			{}

			bool has_project_file() {
				return project_file != ""; }

		};

		struct ProjectSettings;
		

		TEST_CASE("ProjectLoader::  is constructible  without args")
		{
			auto pl = &ProjectLoader();
			REQUIRE(pl != nullptr);
		}

		TEST_CASE("ProjectLoader:: has no project file, if given blank string as starting arg")
		{
			auto pl = TestProjectLoader("");
			REQUIRE_FALSE(pl.has_project_file());
		}

		TEST_CASE("ProjectLoader can set project file, when given")
		{
			auto pl = TestProjectLoader("");
			char const * fake_file = "test file path";

			REQUIRE_FALSE(pl.has_project_file());

			pl.set_project(fake_file);
			REQUIRE_FALSE(pl.has_project_file());

			char const * real_file = "deleteme.delete";
			std::ofstream outfile(real_file);
			outfile << "delete this file, it's only for testing" << std::endl;
			outfile.close();

			pl.set_project(real_file);
			REQUIRE(pl.has_project_file());

			std::remove(real_file);
		}

		TEST_CASE("ProjectLoader parses test file correctly")
		{
			auto pl = TestProjectLoader("");

			char const * cfg_file = "deleteme.delete";
			std::ofstream outfile(cfg_file);
			outfile << "// comment marks are okay\n";
			outfile << " \n";
			outfile << "window_pos: 1, 2 \n";
			outfile << "window_wh: 300, 400 \n";
			outfile << "window_fullscreen: true \n";
			outfile << "window_capture_mouse: true \n";
			outfile << "use_vsync: true \n";
			outfile << " \n";
			outfile.close();

			REQUIRE_NOTHROW(pl.set_project(cfg_file));
			REQUIRE_NOTHROW(pl.parse_project_file());			
			ProjectSettings ps = pl.get_project_settings();

			REQUIRE(ps.window_x == 1);
			REQUIRE(ps.window_y == 2);
			REQUIRE(ps.window_w == 300);
			REQUIRE(ps.window_h == 400);

			REQUIRE(ps.use_fullscreen);
			REQUIRE(ps.capture_mouse);
			REQUIRE(ps.use_vsync);

			REQUIRE(ps.file_paths.size() == 0);

			std::remove(cfg_file);
		}


	#endif // !RUN_UNIT_TESTS

#endif // !PROJECT_LOADER__TST_CC
