
#include "unit_testing_switch.h"
#ifdef RUN_UNIT_TESTS
	#define CATCH_CONFIG_RUNNER
	#include "catch.hpp"
#endif // RUN_UNIT_TESTS

// NOTE: MUST be defined before initializing SDL, if you're using your own main()
// we do it before anything else in the program, to make sure it's "not possible"
// to init SDL before this is defined
#define SDL_MAIN_HANDLED

#include <fstream>
#include <iostream>

// our logging lib
#include <plog/Log.h>

// witchcraft
#include "utility\utility.h"
#include "logging\error_log_manager.h"

#include "resource_manager\resource_manager.h"
#include "render_manager\2d_render_manager.h"

#include "utility\utility.h"
#include "string_constants.h"


int main(int argc, char** argv[])
{
	plog::init(plog::verbose, witchcraft::file_strings::engine_log_file_name.c_str());
	PLOGI << witchcraft::log_strings::engine_start;

	PLOGV << witchcraft::log_strings::engine_unit_tests_start;
	Catch::Session session;
	int numberOfFailures = session.run();
	PLOGV << witchcraft::log_strings::engine_unit_tests_stop;

	PLOGI << witchcraft::log_strings::resource_manager_start;
	auto resource_manager = cResourceManager();
	resource_manager.create_config_files();
	resource_manager.load_from_xml_file("asset/birds.asset");
	resource_manager.load_from_xml_file("asset/buddha.asset");
	resource_manager.load_from_xml_file("asset/person.asset");

	auto render_manager = c2DRenderManager();
	PLOGI << witchcraft::log_strings::render_manager_start;

	std::string title = "Witchcraft";
	bool use_fullscreen = false;

	bool init_successful = render_manager.init(0, 0, 800, 800, use_fullscreen, title.c_str());
	if (init_successful == false)
	{
		PLOGF << witchcraft::log_strings::render_manager_init_failure << "\n" << SDL_GetError(); 
		render_manager.shutdown();
		PLOGV << witchcraft::log_strings::render_manager_stop;
		return EXIT_FAILURE;
	}
	
	bool gameplay_loop_is_running = true;
	SDL_Event window_event;

	PLOGI << witchcraft::log_strings::game_loop_start;
	while (gameplay_loop_is_running)
	{
		if (SDL_PollEvent(&window_event))
		{
			if (SDL_QUIT == window_event.type)
				break;

			if (window_event.type == SDL_KEYDOWN)
			{
				if (window_event.key.keysym.sym == SDLK_ESCAPE)
				{
					PLOGI << witchcraft::log_strings::sdl_break_event_polling;
					gameplay_loop_is_running = false;
				}

			}

			// check moar events
		}

		// do physics update

		// do render update
		render_manager.update();

		// do sound update
	}
	PLOGI << witchcraft::log_strings::game_loop_stop;

	render_manager.shutdown();
	PLOGI << witchcraft::log_strings::render_manager_stop;

	resource_manager.empty_cache();
	PLOGI << witchcraft::log_strings::resource_manager_stop;

	PLOGI << witchcraft::log_strings::engine_stop;
}
