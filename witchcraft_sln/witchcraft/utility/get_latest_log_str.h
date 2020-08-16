#ifndef GET_LATEST_LOG_STR_H
#define GET_LATEST_LOG_STR_H

#include <algorithm>
#include <string>
#include <vector>

#include "utility.h"
#include "../uninitialized_values.h"
#include "../string_constants.h"

namespace witchcraft
{
	namespace configuration
	{
		static std::string get_latest_log_str()
		{
			std::string start_name = witchcraft::file_strings::engine_log_file_name;

			if (utility::file_exists(start_name))
			{
				std::vector<char> vec;

				{ // infile
					std::ifstream infile(start_name);
					if (false == infile.good())
					{
						return std::string(uninit::CSTRING);
					}

					// We're just gonna dump the first three characters,
					// it looks like they're part of a bit header or something
					infile.get();
					infile.get();
					infile.get();

					// example timestamp:  2020-04-05 01:00:11.560 VERB
					while (false == isalpha(infile.peek()))
					{
						vec.push_back(infile.get());
					}
				} // !infile

				std::string end_name(vec.begin(), vec.end());
				end_name += start_name;

				// TODO: consider dropping the <algorithm> dependency (std::replace)
				char const swapping = ':';
				char const swap_to = '`';
				std::replace(end_name.begin(), end_name.end(), swapping, swap_to);
				
				return end_name;
			}

			// else no log file exists for us to move
			return std::string(uninit::CSTRING);
		}
	}
}

#endif // !GET_LATEST_LOG_STR_H
