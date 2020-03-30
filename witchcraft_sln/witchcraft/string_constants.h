#ifndef STRING_CONSTANTS_H
#define STRING_CONSTANTS_H

#include <string>

namespace witchcraft
{
	namespace xml
	{
		static std::string const resource_list = "resources";
		static std::string const resource = "resource";

		static std::string const uuid = "UUID";
		static std::string const resource_type = "type";
		static std::string const resource_name = "name";

		static std::string const file_name = "file";
		static std::string const resource_scope = "scope";

		static std::string const two_d_animation_timing_ms = "timing_ms";
		static std::string const two_d_animation_sequence = "sequence";
	}


	namespace configuration
	{
		static std::string const witchcraft_program_title = "Witchcraft";

		static std::string const witchcraft_cfg = "witchcraft.cfg";
		static std::string const witchcraft_cfg_file_content = "<resources><resource UUID=\"0\" type=\"text\" file=\"witchcraft.cfg\" scope=\"0\">Witchcraft</resource></resources>";

		static std::string const birds_asset = "asset/birds.asset";
		static std::string const birds_asset_file_content = "<resources><resource UUID=\"1\" type=\"graphic\" file=\"asset/birds.png\" scope=\"1\">Winter Birds</resource></resources>";

		static std::string const buddha_asset = "asset/buddha.asset";
		static std::string const buddha_asset_file_content = "<resources><resource UUID=\"2\" type=\"graphic\" file=\"asset/buddha.png\" scope=\"0\">Buddha</resource></resources>";

		static std::string const person_asset = "asset/person.asset";
		static std::string const person_asset_file_content = "<resources>\n\t<resource UUID=\"4\" type=\"graphic\" file=\"asset / person.png\" scope=\"2\"></resource>\n\t<resource UUID=\"100\" type=\"2d_animation\" name=\"idle\" timing_ms=\"250\" sequence=\"1 2 3 1 2 3 1 2 3 3 3 4 4\"></resource>\n</resources>";
	}

	namespace file_strings
	{
		static std::string const engine_errorlog_file_name = "witchcraft.errorlog";
		static std::string const engine_log_file_name = "witchcraft.log";
	}

	namespace log_strings
	{
		static std::string const engine_start = "witchcraft engine start";
		static std::string const engine_stop = "witchcraft engine stop";
						  
		static std::string const engine_unit_tests_start = "engine unit tests start";
		static std::string const engine_unit_tests_stop = "engine unit tests stop";

		static std::string const resource_manager_start = "resource manager start";
		static std::string const resource_manager_stop = "resource manager stop";
		static std::string const resource_manager_meta_load = "metadata loaded ";

		static std::string const render_manager_start = "render manager start";
		static std::string const render_manager_stop = "render manager stop";
		static std::string const render_manager_init_failure = "FAILURE! Could not initialize render manager.  Exiting Program";

		static std::string const sdl_start = "sdl start";
		static std::string const sdl_stop = "sdl stop";
		static std::string const sdl_break_event_polling = "breaking out of sdl event polling";
		static std::string const sdl_begin_shutdown = "begin sdl shutdown";
		static std::string const sdl_init_failure = "FAILURE! Could not initialize SDL";

		static std::string const sdl_window_init = "sdl window init";
		static std::string const sdl_window_init_success = "sdl window created";
		static std::string const sdl_window_init_failure = "FAILURE! Could not initialize an SDL window!";

		static std::string const game_loop_start = "game loop start";
		static std::string const game_loop_stop = "game loop stop";
	}


}

#endif // STRING_CONSTANTS_H
