#ifndef UTILITY_H
#define UTILITY_H

#include <Windows.h>

#include <iostream>
#include <sys/stat.h>
#include <string>

// [NOTE] I read that putting the static methods in a class is "wrong" for C++,
// so instead we're going to use namespaces
namespace utility
{
	static bool fileExists(std::string const & path)
	{
		struct stat buffer;
		return (stat(path.c_str(), &buffer) == 0);
	}

	static void WriteConsole(std::string message)
	{
		std::cout << message;

		// This preprocessor macro is used to send the debug string
		// to VS's output window.  The message already goes to stdout
		// We use the macro to make sure this code isn't even in the
		// release build
		#ifdef _DEBUG
			OutputDebugString(message.c_str());
		#endif
	}
}

#endif
