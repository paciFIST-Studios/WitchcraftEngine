#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <sstream>
#include <string>

// This is going to be our throw macro, to throw a qException class
#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(ErrorNum, ErrorDesc) throw qException(ErrorNum, ErrorDesc, __FILE__, __LINE__);
#endif

// purpose: encapsulate a runtime error
class qException : public std::exception
{
private:
protected:
public:

	int _error_id;
	std::string _error_description;
	std::string _source_file_name;
	int _line_number;
	std::string _readable_message;

	// overrice std::exception::what
	const char* what();

	qException(int ErrorNum, std::string ErrorDesc, std::string SrcFilename, int LineNumber);
	~qException() throw() {}
};

#endif // EXCEPTION_H
