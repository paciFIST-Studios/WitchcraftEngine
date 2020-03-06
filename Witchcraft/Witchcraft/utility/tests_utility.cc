#ifndef TEST_EXCEPTION_CC
#define TEST_EXCEPTION_CC

	// this header controls whether or not the tests run
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS

	#include <cstdio>
	#include <string>
	

	#include "../catch.hpp"
	#include "utility.h"
	
	const std::string OUTFILE_PATH = "delete_me_test_file.del";
	const std::string OUTFILE_DATA = "\nsample data\n";
	
	TEST_CASE("Utility::read_file_to_string returns correct string")
	{
		// create file
		std::ofstream outfile(OUTFILE_PATH.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
		outfile << OUTFILE_DATA;
		outfile.close();
	
		// make sure the data we put into the file, is the data we read back out
		// equality returns 0
		std::string infile = utility::read_file_to_string(OUTFILE_PATH);
		REQUIRE(infile.compare(OUTFILE_DATA.c_str()) == 0);
	
		// be sure to remove the testing file
		REQUIRE(remove(OUTFILE_PATH.c_str()) == 0);
	
		REQUIRE(utility::file_exists(OUTFILE_PATH.c_str()) == false);
	}
	
	TEST_CASE("Utility::file_exists returns true when file exits and false otherwise")
	{
		// create file
		std::ofstream outfile(OUTFILE_PATH.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
		outfile << OUTFILE_DATA;
		outfile.close();

		// can fn detect file?
		REQUIRE(utility::file_exists(OUTFILE_PATH) == true);

		// remove returns 0 for success
		REQUIRE(remove(OUTFILE_PATH.c_str()) == 0);

		// make sure the file is gone, so we know it can detect that
		REQUIRE(utility::file_exists(OUTFILE_PATH.c_str()) == false);
	}


	#endif // RUN_UNIT_TESTS

#endif // TEST_EXCEPTION_CC
