#ifndef UTILITY_H
#define UTILITY_H

	#include <Windows.h>

	#include <fstream>
	#include <sys/stat.h>
	#include <sstream>
	#include <string>
	#include <vector>


	// [NOTE] I read that putting the static methods in a class is "wrong" for C++,
	// so instead we're going to use namespaces
	namespace utility
	{
		static std::string read_file_to_string(std::string const & path)
		{
			// https://stackoverflow.com/questions/116038/how-do-i-read-an-entire-file-into-a-stdstring-in-c
			std::ifstream infile(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);			
			auto file_size = infile.tellg();

			// We can get back a -1 as an error code, and this will cause
			// us to allocate like, INT_MAX bytes for the string (from implicit casting)
			// return a default string instead
			if (file_size < 0)
				return std::string();
			
			infile.seekg(0, std::ios::beg);
			std::vector<char> bytes(static_cast<unsigned int>(file_size));
			infile.read(bytes.data(), file_size);
			infile.close();

			return std::string(bytes.data(), static_cast<unsigned int>(file_size));
		}

		static bool file_exists(std::string const & path)
		{
			struct stat buffer;
			return (stat(path.c_str(), &buffer) == 0);
		}

		static long file_size(std::string const & path)
		{
			// https://stackoverflow.com/questions/5840148/how-can-i-get-a-files-size-in-c
			struct stat stat_buf;
			int rc = stat(path.c_str(), &stat_buf);
			return rc == 0 ? stat_buf.st_size : -1;
		}

		//static void write_to_console(std::string message)
		//{
		//	std::cout << message;
		//
		//	// This preprocessor macro is used to send the debug string
		//	// to VS's output window.  The message already goes to stdout
		//	// We use the macro to make sure this code isn't even in the
		//	// release build
		//	#ifdef _DEBUG
		//		OutputDebugString(message.c_str());
		//	#endif
		//}

		
		template<typename t>
		static t clamp_value_to_uint8(t val)
		{
			if (val < 0)
				return 0;
			else if (val > 255)
				return 255;
			else
				return val;
		}

	}

#endif
