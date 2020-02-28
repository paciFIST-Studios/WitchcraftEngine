#ifndef ERRORLOGMANAGER_H
#define ERRORLOGMANAGER_H
	
	#include <exception>
	#include <fstream>
	#include <iostream>
	#include <iomanip>
	#include <sstream>
	#include <string>
	#include <time.h>
	
	#include "../engine/engine.h"
	
	
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
	
	// purpose: handle and log all exceptions, when they occur, and
	// output to human readable text log
	class cErrorLogManager : public cEngine
	{
	
	public:
		//returns pointer to singleton
		static cErrorLogManager * GetErrorManager();
	
	protected:
		// ctor, encapsulated
		cErrorLogManager();
		// dtor
		virtual ~cErrorLogManager() {}
	
		// static instance
		static cErrorLogManager m_ErrorLogManager;
	
	public:
		//Log file, file buffer
		std::stringstream m_LogBuffer;
	
		void createErrorLog(std::string Filename);
	
		//Commit contents to file
		void flushBuffer();
	
		void closeFile();
	
		void logException(cException e);
	
		std::string getTimeStamp();
	
		// handle to log file itself
		std::ofstream m_LogFile;		
	};

#endif // !ERRORLOGMANAGER_H
