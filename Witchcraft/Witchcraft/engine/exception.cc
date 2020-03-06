#include "exception.h"

cException::cException(int ErrorNum, std::string ErrorDesc, std::string SrcFileName, int LineNum)
{
	m_ErrorNum = ErrorNum;
	m_ErrorDesc = ErrorDesc;
	m_SrcFileName = SrcFileName;
	m_LineNum = LineNum;

	// write properties to a "human readable" string
	std::stringstream ErrorStr;

	ErrorStr << "ErrorNum: " << m_ErrorNum
		<< "\nErrorDesc: " << m_ErrorDesc
		<< "\nSrcFileName: " << m_SrcFileName
		<< "\nLineNumber: " << m_LineNum
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