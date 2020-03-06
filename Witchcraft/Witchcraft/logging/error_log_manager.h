#ifndef ERRORLOGMANAGER_H
#define ERRORLOGMANAGER_H
	
	#include <fstream>
	#include <iostream>
	#include <iomanip>
	#include <sstream>
	#include <string>
	#include <time.h>
	
	#include "../engine/engine.h"
	#include "../exception.h"
	
	

	
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
