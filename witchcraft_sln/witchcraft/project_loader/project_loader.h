#ifndef PROJECT_LOADER_H
#define PROJECT_LOADER_H

#include <iostream>
#include <tuple>
#include <map>

#include "../engine/engine_object.h"

#include "../message_bus/message_bus.h"
#include "../utility/utility.h"

struct ProjectSettings
{
	int window_x;
	int window_y;
	int window_w;
	int window_h;

	bool use_fullscreen;
	bool capture_mouse;
	bool use_vsync;

	std::vector<std::string> file_paths;
};

#define PL ProjectLoader

class ProjectLoader : public EngineObjectBase
{
protected:
	std::string const COMMENT_MARK = "//";
	std::string const DELIMITER = ":";
	std::string const COMMA = ",";
	std::string const WHITESPACE = "";

	std::string const WINDOW_POS = "window_pos:";
	std::string const WINDOW_WH = "window_wh:";
	std::string const WINDOW_FULLSCREEN = "window_fullscreen:";
	std::string const WINDOW_CAPTURE_MOUSE = "window_capture_mouse:";
	std::string const USE_VSYNC = "use_vsync:";
	std::string const PATH = "path:";

private:
protected:
	MessageBus * message_bus = nullptr;

	std::map<char const *, char const *> files;

	ProjectSettings project_settings;

	std::string project_file;

	inline bool has_tag(std::string const & tag, std::string const & line)
	{
		return line.find(tag) != std::string::npos;
	}

	bool parse_for_bool(std::string const & tag, std::string const & line)
	{
		size_t clip_end = line.find(tag);
		auto value = line.substr(clip_end + 1);
		if (has_tag("true", value)) {
			return true; }

		return false;
	}

	std::tuple<int, int> parse_for_two_ints(std::string const & delimiters, std::string const & line)
	{
		size_t offset = line.find_first_of(PL::DELIMITER) + 1;
		auto values   = line.substr(offset);

		offset = values.find_first_of(PL::COMMA);
		auto first = values.substr(0, offset);

		offset += 1;
		auto second = values.substr(offset);
		
		return std::make_tuple(std::stoi(first), std::stoi(second));
	}

public:
	ProjectLoader() {}
	~ProjectLoader() {}

	ProjectLoader(std::string filepath)
		: project_file(filepath)
	{}

	void init_component()
	{
		// load the project here
	}

	void set_project(std::string filepath)
	{
		if (utility::file_exists(filepath))
		{
			project_file = filepath;
		}
	}

	ProjectSettings get_project_settings() const
	{
		return project_settings;
	}

	void parse_project_file()
	{
		std::fstream infile;
		infile.open(project_file, std::ios::in);
		if (infile.is_open())
		{
			project_settings = ProjectSettings();

			std::string line;
			while (getline(infile, line))
			{
				// note, the args are swapped here
				if (has_tag(line, PL::WHITESPACE))
				{
					continue;
				}
				else if (has_tag(PL::COMMENT_MARK, line))
				{
					continue;
				}
				else if (has_tag(PL::WINDOW_POS, line))
				{
					auto tup = parse_for_two_ints(PL::COMMA, line);			
					project_settings.window_x = std::get<0>(tup);
					project_settings.window_y = std::get<1>(tup);
				}
				else if (has_tag(PL::WINDOW_WH, line))
				{
					auto tup = parse_for_two_ints(PL::COMMA, line);
					project_settings.window_w = std::get<0>(tup);
					project_settings.window_h = std::get<1>(tup);
				}
				else if (has_tag(PL::WINDOW_FULLSCREEN, line))
				{
					project_settings.use_fullscreen = parse_for_bool(PL::WINDOW_FULLSCREEN, line);
				}
				else if (has_tag(PL::WINDOW_CAPTURE_MOUSE, line))
				{
					project_settings.capture_mouse = parse_for_bool(PL::WINDOW_CAPTURE_MOUSE, line);
				}
				else if (has_tag(PL::USE_VSYNC, line))
				{
					project_settings.use_vsync = parse_for_bool(PL::USE_VSYNC, line);
				}
				else if (has_tag(PL::PATH, line))
				{
					auto pos = line.find(PL::PATH) + PL::PATH.size() + 1;
					auto s = line.substr(pos);
					// https://www.techiedelight.com/remove-whitespaces-string-cpp/
					s.erase(std::remove_if(s.begin(), s.end(), std::isspace), s.end());					
					project_settings.file_paths.emplace_back(s);
				}

			} // while-loop

			infile.close();
		} // if infile open
	}
};

#endif // !PROJECT_LOADER_H