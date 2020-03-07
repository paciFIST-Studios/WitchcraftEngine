
#include "unit_testing_switch.h"
#ifdef RUN_UNIT_TESTS
	#define CATCH_CONFIG_RUNNER
	#include "catch.hpp"
#endif // RUN_UNIT_TESTS

#include <fstream>
#include <iostream>

#include "utility\utility.h"
#include "logging\error_log_manager.h"

#include "resource_manager\resource_manager.h"


const std::string ErrorLogFileName = "witchcraft_errors.log";

int main(int argc, char** argv[])
{
	Catch::Session session;
	int numberOfFailures = session.run();

	cResourceManager * rm = &cResourceManager();


	const std::string TEST_FILE = "test_delete_me.del";
	const std::string TEST_FILE_DATA = "<resources><text>resource data</text></resources>";
	std::ofstream outfile(TEST_FILE, std::ios::out | std::ios::binary | std::ios::trunc);
	outfile << TEST_FILE_DATA;
	outfile.close();

	rm->load_from_xml_file(TEST_FILE);
	
	char c;
	std::cin >> c;


	//cErrorLogManager * log = cErrorLogManager::get_error_manager();
	//log->create_error_log(ErrorLogFileName);
	//
	//try
	//{
	//	THROW_EXCEPTION(666, "Ellie loves satan");
	//}
	//catch (cException & e)
	//{
	//	MessageBoxA(NULL, e.what(), "", MB_OK);
	//	log->_log_buffer << "\n\n*************** ERROR ***************\n";
	//	log->flush_buffer();
	//	log->log_exception(e);
	//	log->_log_buffer << "\n\n*************************************";
	//	log->flush_buffer();
	//}
	//
	//log->close_file();
	
}
