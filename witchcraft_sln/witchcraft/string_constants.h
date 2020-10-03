#ifndef STRING_CONSTANTS_H
#define STRING_CONSTANTS_H


namespace witchcraft
{
	namespace xml
	{
		static char const * delimiter = ",";

		static char const * resource_list = "resources";
		static char const * resource = "resource";

		static char const * UUID = "UUID";
		static char const * TYPE = "type";
		static char const * NAME = "name";

		static char const * FILEPATH = "filepath";
		static char const * SCOPE = "scope";
		static char const * VERSION = "version";
		static char const * WITCHCRAFT = "witchcraft";

		// NOTE: valid data for sprite atlas is: "int width, int height";
		// these refer to the standard width and height of tiles in this atlas
		static char const * sprite_atlas = "sprite_atlas";
		static char const * atlas_uuid = "atlas_uuid";
		static char const * atlas_name = "atlas_name";

		static char const * animation_2d_type = "animation_2d";
		static char const * animation_2d_ms_per_frame = "ms_per_frame";
		static char const * animation_2d_sequence = "sequence";

		static char const * SHADER_TYPE = "SHADER_TYPE";
	}

	namespace configuration
	{
		static char const * witchcraft_program_title = "Witchcraft";

		static char const * witchcraft_cfg = "witchcraft.cfg";
		static char const * witchcraft_cfg_file_content = "<resources><resource UUID=\"0\" type=\"text\" file=\"witchcraft.cfg\" scope=\"0\">Witchcraft</resource></resources>";

		static char const * birds_png = "asset/birds.png";
		static char const * birds_asset = "asset/birds.asset";
		static char const * birds_asset_file_content = "<resources><resource UUID=\"1\" type=\"graphic\" file=\"asset/birds.png\" scope=\"1\">Winter Birds</resource></resources>";

		static char const * buddha_png = "asset/buddha.png";
		static char const * buddha_asset = "asset/buddha.asset";
		static char const * buddha_asset_file_content = "<resources><resource UUID=\"2\" type=\"graphic\" file=\"asset/buddha.png\" scope=\"0\">Buddha</resource></resources>";

		static char const * person_png = "asset/person.png";
		static char const * person_asset = "asset/person.asset";
		static char const * person_asset_file_content = "<resources>\n\t<resource UUID=\"4\" type=\"graphic\" file=\"asset/person.png\" scope=\"2\"></resource>\n\t<resource UUID=\"100\" type=\"2d_animation\" name=\"idle\" timing_ms=\"250\" sequence=\"1 2 3 1 2 3 1 2 3 3 3 4 4\"></resource>\n</resources>";
	}

	namespace file_strings
	{
		static char const * engine_errorlog_file_name = "witchcraft.errorlog";
		static char const * engine_log_file_name = "witchcraft.log";
	}

	namespace banners
	{
		// I want there to be ascii art in the logs, that goes here
		static char const * unit_test_banner = "";
	}

	namespace log_strings
	{
		static char const * audio_manager_start = "audio manager start";
		static char const * audio_manager_stop = "audio_manager_stop";

		static char const * debug_console = "debug console start";

		static char const * engine_startup = "witchcraft engine startup";
		static char const * engine_running = "witchcraft engine running";
		static char const * engine_shutdown = "witchcraft engine shutdown";					  

		static char const * engine_unit_tests_start = "engine unit tests start";
		static char const * engine_unit_tests_stop = "engine unit tests stop";

		static char const * game_loop_start = "game loop start";
		static char const * game_loop_stop = "game loop stop";

		static char const * gameplay_manager_start = "gameplay manager start";
		static char const * gameplay_manager_stop  = "gameplay manager stop";

		static char const * message_bus_start = "message bus start";
		static char const * message_but_stop = "message bus stop";
		static char const * message_bus_init_failure = "FAILURE! Could not initialize message bus.  Exiting program.";

		static char const * project_loader_start = "project loader start";

		static char const * resource_manager_start	= "resource manager start";
		static char const * resource_manager_stop	= "resource manager stop";
		static char const * resource_manager_meta_load = "metadata loaded ";

		static char const * render_manager_start = "render manager start";
		static char const * render_manager_stop = "render manager stop";
		static char const * render_manager_system_init_start = "render manager system init start";
		static char const * render_manager_system_init_end = "render manager system init end";
		static char const * render_manager_init_failure = "FAILURE! Could not initialize render manager.  Exiting Program";

		static char const * scene_manager_start = "scene manager start";
		static char const * scene_manager_stop = "scene manager stop";
		static char const * scene_manager_init_failure = "FAILURE! Could not initialize scene manager.  Exiting Program.";

		static char const * sdl_start = "sdl start";
		static char const * sdl_ok = "sdl ok";
		static char const * sdl_stop = "sdl stop";
		static char const * sdl_break_event_polling = "breaking out of sdl event polling";
		static char const * sdl_begin_shutdown = "begin sdl shutdown";
		static char const * sdl_init_failure = "FAILURE! Could not initialize SDL";

		static char const * sdl_window_create = "sdl window create";
		static char const * sdl_window_ok = "sdl window ok";	
		static char const * sdl_window_fail = "FAILURE: sdl window: could not create sdl window";	
		static char const * sdl_window_init_failure = "FAILURE! Could not initialize an SDL window!";

		static char const * opengl_context_create = "opengl context create";
		static char const * opengl_context_ok = "opengl context ok";

		static char const * opengl_glew_start = "glew (opengl), start";
		static char const * opengl_glew_ok = "glew (opengl), ok";
		static char const * opengl_glew_init_failure = "FAILURE: glew (opengl): could not initialize glew";
	}
}

#endif // STRING_CONSTANTS_H
