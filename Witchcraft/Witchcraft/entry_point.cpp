#include <windows.h>

#include  "logging\error_log_manager.h"

const std::string ErrorLogFileName = "witchcraft_error_log.txt";

int main(int argc, char** argv[])
{
	OutputDebugString("\n\nEntry Point for program\n\n");

	cErrorLogManager * log = cErrorLogManager::GetErrorManager();
	log->createErrorLog(ErrorLogFileName);

	try
	{
		THROW_EXCEPTION(666, "Ellie loves satan");
	}
	catch (cException & e)
	{
		MessageBoxA(NULL, e.what(), "", MB_OK);
		log->m_LogBuffer << "\n\n*************** ERROR ***************\n";
		log->flushBuffer();
		log->logException(e);
		log->m_LogBuffer << "\n\n*************************************";
		log->flushBuffer();
	}

	log->closeFile();
	
}
