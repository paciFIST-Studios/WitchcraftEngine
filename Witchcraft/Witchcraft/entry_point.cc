
#include "unit_testing_switch.h"
#ifdef RUN_UNIT_TESTS
	#define CATCH_CONFIG_RUNNER
	#include "catch.hpp"
#endif // RUN_UNIT_TESTS


#include <iostream>

#include "utility\utility.h"
#include "logging\error_log_manager.h"

#include "resource_manager\resource_manager.h"


const std::string ErrorLogFileName = "witchcraft_errors.log";

int main(int argc, char** argv[])
{
	Catch::Session session;
	int numberOfFailures = session.run();

	
	cResourceManager *rm = &cResourceManager();
	
	char c;
	std::cin >> c;


	//cErrorLogManager * log = cErrorLogManager::GetErrorManager();
	//log->createErrorLog(ErrorLogFileName);
	//
	//try
	//{
	//	THROW_EXCEPTION(666, "Ellie loves satan");
	//}
	//catch (cException & e)
	//{
	//	MessageBoxA(NULL, e.what(), "", MB_OK);
	//	log->m_LogBuffer << "\n\n*************** ERROR ***************\n";
	//	log->flushBuffer();
	//	log->logException(e);
	//	log->m_LogBuffer << "\n\n*************************************";
	//	log->flushBuffer();
	//}
	//
	//log->closeFile();
	
}
