
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

#include "utility\utility.h"
#include "logging\error_log_manager.h"

#include "resource_manager\resource_manager.h"
#include "render_manager/SDL2_2D_render_manager.h"
#include "utility/utility.h"


const std::string ErrorLogFileName = "witchcraft_errors.log";

int main(int argc, char** argv[])
{
	ULOG("\n[Witchcraft]: BEGIN");
	ULOG("\n[Witchcraft]::[Unit Tests]: BEGIN\n\n");

	Catch::Session session;
	int numberOfFailures = session.run();

	ULOG("[Witchcraft]::[Unit Tests]: END\n");
	ULOG("\n[Witchcraft]::[Init]: BEGIN");

	cResourceManager * resource_manager = new cResourceManager();
	resource_manager->create_config_files();
	resource_manager->load_from_xml_file("asset/birds.asset");
	resource_manager->load_from_xml_file("asset/buddha.asset");
	resource_manager->load_from_xml_file("asset/person.asset");

	ULOG("\n[Witchcraft]::[Init]::[SDL2 Render Manager]: Create");

	cSDL2RenderManager * render_manager = new cSDL2RenderManager();

	ULOG("\n[Witchcraft]::[Init]::[SDL2 Render Manager]: Init");

	std::string title = "Witchcraft";
	bool use_fullscreen = false;

	bool init_success = render_manager->init(0, 0, 800, 800, use_fullscreen, title.c_str());
	if (init_success)
	{
		ULOG("\n[Witchcraft]::[Init]::[SDL2 Render Manager]: Init Success");

		// For testing, we're setting the background to a color
		int r = 70;
		int g = 0;
		int b = 70;

		render_manager->set_surface_RGB(r, g, b, nullptr);
	}
	else
	{
		ULOG("\n[Witchcraft]::[Init]::[SDL2 Render Manager]: Init Fail");
		render_manager->shutdown();
		return EXIT_FAILURE;
	}

	bool gameplay_loop_is_running = true;
	SDL_Event window_event;
	while (gameplay_loop_is_running)
	{
		if (SDL_PollEvent(&window_event))
		{
			if (SDL_QUIT == window_event.type)
				break;

			if (window_event.type == SDL_KEYDOWN)
			{
				if (window_event.key.keysym.sym == SDLK_ESCAPE)
					gameplay_loop_is_running = false;
			}

			// check moar events
		}

		// do physics update

		// do render update
		render_manager->update();

		// do sound update
	}

	render_manager->shutdown();

	ULOG("\n[Witchcraft]: END");
}
