#include "error_log_manager.h"

// I have no idea what this even means
cErrorLogManager cErrorLogManager::_error_log_manager;

cErrorLogManager::cErrorLogManager(){}

cErrorLogManager * cErrorLogManager::get_error_manager()
{
	return &_error_log_manager;
}

void cErrorLogManager::create_error_log(std::string filename)
{
	// take the current one, and rename it to a new timestamp
	// then launch an optional thread to copy that file to somewhere
	_log_file.open(filename.c_str());
}

void cErrorLogManager::flush_buffer()
{
	_log_file << _file_buffer.str();
	_log_file.flush();
	_file_buffer.str("");
}

void cErrorLogManager::close_file()
{
	_log_file.close();
}

void cErrorLogManager::log_exception(cException e)
{
	_file_buffer << get_timestamp() << "\n" << e.what();
	flush_buffer();
}

std::string cErrorLogManager::get_timestamp()
{
		std::stringstream time_str;

		time_t ct_time;
		time(&ct_time);

		struct tm p_time;
		localtime_s(&p_time, &ct_time);
		
		time_str << std::setw(2) << std::setfill('0') << p_time.tm_hour << ":";
		time_str << std::setw(2) << std::setfill('0') << p_time.tm_min  << ":";
		time_str << std::setw(2) << std::setfill('0') << p_time.tm_sec;

		return time_str.str();
}
