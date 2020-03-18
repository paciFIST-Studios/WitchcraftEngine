#include "exception.h"

cException::cException(int ErrorNum, std::string ErrorDesc, std::string SrcFileName, int LineNum)
	: _error_id(ErrorNum)
	, _error_description(ErrorDesc)
	, _source_file_name(SrcFileName)
	, _line_number(LineNum)
{
	// write properties to a "human readable" string
	std::stringstream ErrorStr;

	ErrorStr << "ErrorNum: " << _error_id
		<< "\nErrorDesc: "   << _error_description
		<< "\nSrcFileName: " << _source_file_name
		<< "\nLineNumber: "  << _line_number
		<< "/n";

	_readable_message = ErrorStr.str();
}

char const * cException::what()
{
	return _error_description.c_str();
}

// example code
//	try 
//	{
//		THROW_EXCEPTION(1, "Test Error");
//	}
//	catch (cException & e)
//	{
//		e.what();
//	}
