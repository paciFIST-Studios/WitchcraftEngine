#ifndef CONSOLE__TST_CC
#define CONSOLE__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../console/console.h"
		
		TEST_CASE(" Console::ctor   constructs console ")
		{
			Console * c1 = &Console();
			REQUIRE(c1 != nullptr);
		}
	
		TEST_CASE("  Console::get_commands()  returns command list")
		{
			auto c = Console();
			auto commands = c.get_commands();

			REQUIRE(commands[0] == "HELP");
			REQUIRE(commands[1] == "HISTORY");
			REQUIRE(commands[2] == "CLEAR");
		}

		TEST_CASE(" Console::get_contents()  returns list of contents")
		{
			std::string s = "test string";

			auto c = Console();
			c.add_log(s);
			auto contents = c.get_contents();

			REQUIRE(contents[0] == s);
		}

		TEST_CASE(" Console::clear_contents()  clears internal contents vector")
		{
			std::string s = "test string";
			auto c = Console();
			c.add_log(s);

			REQUIRE(c.get_contents().size() == 1);
			REQUIRE(c.get_contents()[0] == s);
			c.clear_contents();

			REQUIRE(c.get_contents().size() == 0);
		}

		TEST_CASE(" Console::add_log()  adds string to internal contents vector")
		{
			std::string s = "test string";
			auto c = Console();

			REQUIRE(c.get_contents().size() == 0);
			c.add_log(s);

			REQUIRE(c.get_contents().size() == 1);
			REQUIRE(c.get_contents()[0] == s);
		}

	#endif // RUN_UNIT_TESTS

#endif // !CONSOLE__TST_CC


