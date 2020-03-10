
#include "unit_testing_switch.h"
#ifdef RUN_UNIT_TESTS
	#define CATCH_CONFIG_RUNNER
	#include "catch.hpp"
#endif // RUN_UNIT_TESTS

#include <fstream>
#include <iostream>

#include "utility\utility.h"
#include "logging\error_log_manager.h"

#include "resource_manager\resource_manager.h"
#include "render_manager/SDL2_2D_render_manager.h"

const std::string ErrorLogFileName = "witchcraft_errors.log";

int main(int argc, char** argv[])
{
	std::cout << "\n[Witchcraft]: BEGIN";
	std::cout << "\n[Witchcraft]::[Unit Tests]: BEGIN\n\n";

	Catch::Session session;
	int numberOfFailures = session.run();

	std::cout << "\n[Witchcraft]::[Unit Tests]: END";

	std::cout << "\n[Witchcraft]::[Init]: BEGIN";
	std::cout << "\n[Witchcraft]::[Init]::[SDL2 Render Manager]: Create";

	cSDL2RenderManager * renderManager = &cSDL2RenderManager();

	std::cout << "\n[Witchcraft]::[Init]::[SDL2 Render Manager]: Init";

	bool init_success = renderManager->init();
	if (init_success)
	{
		std::cout << "\n[Witchcraft]::[Init]::[SDL2 Render Manager]: Init Success";
	}
	else
	{
		std::cout << "\n[Witchcraft]::[Init]::[SDL2 Render Manager]: Init Fail";
	}

	SDL_Event window_event;
	while (true)
	{
		if (SDL_PollEvent(&window_event))
		{
			if (SDL_QUIT == window_event.type)
				break;

			if (window_event.type == SDL_KEYDOWN)
			{
				if (window_event.key.keysym.sym == SDLK_ESCAPE)
					break;
			}
				
				
		}
	}

	// main loop here?


	renderManager->shutdown();

	std::cout << "\n[Witchcraft]: END";


	char c;
	std::cin >> c;

}
