#ifndef ERRORLOGMANAGER_H
#define ERRORLOGMANAGER_H
	
	#include <fstream>
	#include <iostream>
	#include <iomanip>
	#include <sstream>
	#include <string>
	#include <time.h>
	
	#include "../engine/engine.h"
	#include "../engine/exception.h"
	
	// purpose: handle and log all exceptions, when they occur, and
	// output to human readable text log
	class cErrorLogManager : public cEngine
	{
	
	public:
		//returns pointer to singleton
		static cErrorLogManager * get_error_manager();
	
	protected:
		// ctor, encapsulated
		cErrorLogManager();
		// dtor
		virtual ~cErrorLogManager() {}
	
		// static instance
		static cErrorLogManager _error_log_manager;
	
	public:
		//Log file, file buffer
		std::stringstream _log_buffer;
	
		void create_error_log(std::string Filename);
	
		//Commit contents to file
		void flush_buffer();
	
		void close_file();
	
		void log_exception(cException e);
	
		std::string get_timestamp();
	
		// handle to log file itself
		std::ofstream _log_file;		
	};

#endif // !ERRORLOGMANAGER_H
