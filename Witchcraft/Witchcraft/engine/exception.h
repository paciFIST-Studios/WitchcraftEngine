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

	int m_ErrorNum;
	std::string m_ErrorDesc;
	std::string m_SrcFileName;
	int m_LineNum;
	std::string m_ReadableMessage;

	// overrice std::exception::what
	const char* what();

	cException(int ErrorNum, std::string ErrorDesc, std::string SrcFilename, int LineNumber);
	~cException() throw() {}
};


#endif // EXCEPTION_H