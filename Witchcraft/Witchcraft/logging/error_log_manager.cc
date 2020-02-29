#include "error_log_manager.h"


// - cException  ----------------------------------------------------------------------------------

cException::cException(int ErrorNum, std::string ErrorDesc, std::string SrcFileName, int LineNum)
{
	m_ErrorNum = ErrorNum;
	m_ErrorDesc = ErrorDesc;
	m_SrcFileName = SrcFileName;
	m_LineNum = LineNum;

	// write properties to a "human readable" string
	std::stringstream ErrorStr;

	ErrorStr << "ErrorNum: "      << m_ErrorNum 
			 << "\nErrorDesc: "   << m_ErrorDesc 
			 << "\nSrcFileName: " << m_SrcFileName 
			 << "\nLineNumber: "  << m_LineNum 
			 << "/n";

	m_ReadableMessage = ErrorStr.str();
}

char const * cException::what()
{
	return m_ErrorDesc.c_str();
}


// try 
// {
// 	THROW_EXCEPTION(1, "Test Error");
// }
// catch (cException & e)
// {
// 	e.what();
// }




// - Log Manager ----------------------------------------------------------------------------------

cErrorLogManager cErrorLogManager::m_ErrorLogManager;

cErrorLogManager::cErrorLogManager(){}

cErrorLogManager * cErrorLogManager::GetErrorManager()
{
	return &m_ErrorLogManager;
}

void cErrorLogManager::createErrorLog(std::string filename)
{
	// take the current one, and rename it to a new timestamp, then launch an optional thread to copy that file to a specific place
	m_LogFile.open(filename.c_str());
}

void cErrorLogManager::flushBuffer()
{
	m_LogFile << m_LogBuffer.str();
	m_LogFile.flush();
	m_LogBuffer.str("");
}

void cErrorLogManager::closeFile()
{
	m_LogFile.close();
}

void cErrorLogManager::logException(cException e)
{
	m_LogBuffer << getTimeStamp() << "\n" << e.what();
	flushBuffer();
}

std::string cErrorLogManager::getTimeStamp()
{
		std::stringstream TimeStr;

		time_t ctTime;
		time(&ctTime);

		struct tm pTime;
		localtime_s(&pTime, &ctTime);
		
		TimeStr << std::setw(2) << std::setfill('0') << pTime.tm_hour << ":";
		TimeStr << std::setw(2) << std::setfill('0') << pTime.tm_min  << ":";
		TimeStr << std::setw(2) << std::setfill('0') << pTime.tm_sec;

		return TimeStr.str();
}
