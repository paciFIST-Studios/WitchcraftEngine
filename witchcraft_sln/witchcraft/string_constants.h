#ifndef STRING_CONSTANTS_H
#define STRING_CONSTANTS_H

#include <string>

namespace witchcraft
{
	namespace log_strings
	{
		std::string const engine_start = "witchcraft engine start";
		std::string const engine_stop = "witchcraft engine stop";
						  
		std::string const engine_unit_tests_start = "engine unit tests start";
		std::string const engine_unit_tests_stop = "engine unit tests stop";

		std::string const resource_manager_start = "resource manager start";
		std::string const resource_manager_stop = "resource manager stop";
		std::string const resource_manager_meta_load = "metadata loaded ";

		std::string const render_manager_start = "render manager start";
		std::string const render_manager_stop = "render manager stop";
		std::string const render_manager_init_failure = "FAILURE! Could not initialize render manager.  Exiting Program";

		std::string const sdl_start = "sdl start";
		std::string const sdl_stop = "sdl stop";
		std::string const sdl_break_event_polling = "breaking out of sdl event polling";
		std::string const sdl_begin_shutdown = "begin sdl shutdown";
		std::string const sdl_init_failure = "FAILURE! Could not initialize SDL";

		std::string const sdl_window_init = "sdl window init";
		std::string const sdl_window_init_success = "sdl window created";
		std::string const sdl_window_init_failure = "FAILURE! Could not initialize an SDL window!";

		std::string const game_loop_start = "game loop start";
		std::string const game_loop_stop = "game loop stop";




	}
}

#endif // STRING_CONSTANTS_H