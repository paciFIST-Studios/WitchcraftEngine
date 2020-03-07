#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <sstream>
#include <string>

// This is going to be our throw macro, to throw a cException class
#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(ErrorNum, ErrorDesc) throw cException(ErrorNum, ErrorDesc, __FILE__, __LINE__);
#endif


// purpose: encapsulate a runtime error
class cException : public std::exception
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

	cException(int ErrorNum, std::string ErrorDesc, std::string SrcFilename, int LineNumber);
	~cException() throw() {}
};


#endif // EXCEPTION_H