#ifndef TEST_ERROR_LOG_MANAGER_CC
#define TEST_ERROR_LOG_MANAGER_CC

	// this header will be whitespace when the testing is switched off
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS

		#include "../catch.hpp"
		#include "error_log_manager.h"

		#include "../utility/utility.h"
		

		const std::string ERROR_LOG_FILE_PATH = "delete_me_test_file.del";
		const std::string ERROR_LOG_CONTENT = "*********************\n* ERROR LOG:\n*********************\n*\n* error log test\n*********************";

		static void removePreviousTestingFiles()
		{
			if (utility::fileExists(ERROR_LOG_FILE_PATH))
			{
				int delete_result = remove(ERROR_LOG_FILE_PATH.c_str());

				if (delete_result != 0)
				{
					FAIL("Could not satisfy testing pre-condition: Old test data could not be deleted");
				}

				if (utility::fileExists(ERROR_LOG_FILE_PATH))
				{
					FAIL("Could not satisfy testing pre-condition: Old test data could not be deleted");
				}
			}
		}
	   
		// This test case creates a log, and closes it without adding any data.
		// Then we attempt to verify the creation of the log.  If it succeeds, 
		// we certify that the error logger can create log files.
		TEST_CASE("ErrorLogManager: Creates Error Log File")
		{
			removePreviousTestingFiles();

			cErrorLogManager * log = cErrorLogManager::GetErrorManager();
			log->createErrorLog(ERROR_LOG_FILE_PATH);
			log->closeFile();

			REQUIRE(utility::fileExists(ERROR_LOG_FILE_PATH));

			log = nullptr;
		}

		// This test case creates a log, adds data to the buffer
		// , but does not flush the buffer, and then closes the log.
		// We check if the log contains the correct information (ie: none)
		// On success, we certify that the error logger does NOT flush automatically
		TEST_CASE("ErrorLogManager: Flush() must be called to write data to file")
		{
			removePreviousTestingFiles();

			cErrorLogManager * log = cErrorLogManager::GetErrorManager();
			log->createErrorLog(ERROR_LOG_FILE_PATH);
			log->m_LogBuffer << ERROR_LOG_CONTENT;
			log->closeFile();

			std::string file_content = utility::readFileToString(ERROR_LOG_FILE_PATH);

			REQUIRE(file_content.compare("") == 0); // true
			REQUIRE(file_content.compare(ERROR_LOG_CONTENT) != 0); // false
		}

		// This test case creates a log, adds data to the buffer
		// , flushes the buffer, and closes the log
		// We check if the log contains the correct information
		// On success, we certify that the error logger can write to a log file correctly
		TEST_CASE("ErrorLogManager: Writes to Error Log File")
		{
			removePreviousTestingFiles();

			cErrorLogManager * log = cErrorLogManager::GetErrorManager();
			log->createErrorLog(ERROR_LOG_FILE_PATH);
			log->m_LogBuffer << ERROR_LOG_CONTENT;
			log->flushBuffer();
			log->closeFile();

			std::string file_content = utility::readFileToString(ERROR_LOG_FILE_PATH);

			REQUIRE(file_content.compare(ERROR_LOG_CONTENT));

			log = nullptr;
		}


	#endif // RUN_UNIT_TESTS

#endif // !TEST_ERROR_LOG_MANAGER_CC

