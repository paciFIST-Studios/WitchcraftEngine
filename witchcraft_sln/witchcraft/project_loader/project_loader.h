#ifndef PROJECT_LOADER_H
#define PROJECT_LOADER_H

#include <iostream>
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
	//std::map<char const *, char const *> file_paths;
};

#define PL ProjectLoader

class ProjectLoader : public qEngineObject
{
protected:
	char const * COMMENT_MARK = "//";
	char const * WHITESPACE = "";

	char const * WINDOW_POS = "window_pos:";
	char const * WINDOW_WH = "window_wh:";
	char const * WINDOW_FULLSCREEN = "window_fullscreen:";
	char const * WINDOW_CAPTURE_MOUSE = "window_capture_mouse:";
	char const * USE_VSYNC = "use_vsync:";
	char const * PATH = "path:";

private:
protected:

	MessageBus * message_bus = nullptr;

	char const * project_file = nullptr;

	std::map<char const *, char const *> files;

	ProjectSettings project_settings;

	inline bool has_tag(char const & tag, std::string const & line)
	{
		return line.find(tag) != std::string::npos;
	}

	bool parse_for_bool(char const & tag, std::string const & line)
	{
		size_t clip_end = line.find(tag);
		auto s = line.substr(clip_end + 1);
		return false;
	}

	std::string parse_for_string(char const & tag, std::string const & line)
	{
		return "";
	}
	

public:
	ProjectLoader() {}
	~ProjectLoader() {}

	ProjectLoader(MessageBus * mb)
		: message_bus(mb)
	{}

	void set_project(char const * filepath)
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
				if (has_tag(*PL::WHITESPACE, line))
				{
					continue;
				}
				else if (has_tag(*PL::COMMENT_MARK, line))
				{
					continue;
				}
				else if (has_tag(*PL::WINDOW_POS, line))
				{
					// 0 + len(tag) + 1
					auto s = line.substr(line.find(*PL::WINDOW_POS));
					int i = 0;
				}
				else if (has_tag(*PL::WINDOW_WH, line))
				{
				}
				else if (has_tag(*PL::WINDOW_FULLSCREEN, line))
				{
					project_settings.use_fullscreen = parse_for_bool(*PL::WINDOW_FULLSCREEN, line);
				}
				else if (has_tag(*PL::WINDOW_CAPTURE_MOUSE, line))
				{
					project_settings.use_fullscreen = parse_for_bool(*PL::WINDOW_CAPTURE_MOUSE, line);
				}
				else if (has_tag(*PL::USE_VSYNC, line))
				{
					project_settings.use_fullscreen = parse_for_bool(*PL::USE_VSYNC, line);
				}
				else if (has_tag(*PL::PATH, line))
				{
					auto path = parse_for_string(*PL::PATH, line);
					// all suplied paths are at least file which exist
					if (utility::file_exists(path)){
						project_settings.file_paths.push_back(path);}
				}

			} // while-loop

			infile.close();
		} // if infile open
	}
};

#endif // !PROJECT_LOADER_H