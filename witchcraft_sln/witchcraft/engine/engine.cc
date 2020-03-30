#include "engine.h"


void cEngine::startup()
{
	PLOGI << witchcraft::log_strings::engine_startup;
	_current_engine_state = EEngineState::STARTUP;

	// messaging layer


	PLOGI << witchcraft::log_strings::resource_manager_start;
	resource = std::make_unique<cResourceManager>();
	

	//auto render_manager = c2DRenderManager();
	//PLOGI << witchcraft::log_strings::render_manager_start;
}

void cEngine::run()
{
	PLOGI << witchcraft::log_strings::engine_running;
	_current_engine_state = EEngineState::RUNNING;


	//std::string title = witchcraft::configuration::witchcraft_program_title;
	//bool use_fullscreen = false;

	//bool init_successful = render_manager.init(0, 0, 800, 800, use_fullscreen, title.c_str());
	//if (init_successful == false)
	//{
	//	PLOGF << witchcraft::log_strings::render_manager_init_failure << "\n" << SDL_GetError(); 
	//	render_manager.shutdown();
	//	PLOGV << witchcraft::log_strings::render_manager_stop;
	//	return EXIT_FAILURE;
	//}
	//
	//bool gameplay_loop_is_running = true;
	//SDL_Event window_event;
	//
	//PLOGI << witchcraft::log_strings::game_loop_start;
	//while (gameplay_loop_is_running)
	//{
	//	if (SDL_PollEvent(&window_event))
	//	{
	//		if (SDL_QUIT == window_event.type)
	//			break;
	//
	//		if (window_event.type == SDL_KEYDOWN)
	//		{
	//			if (window_event.key.keysym.sym == SDLK_ESCAPE)
	//			{
	//				PLOGI << witchcraft::log_strings::sdl_break_event_polling;
	//				gameplay_loop_is_running = false;
	//			}
	//
	//		}
	//
	//		// check moar events
	//	}
	//
	//	// do physics update
	//
	//	// do render update
	//	render_manager.update();
	//
	//	// do sound update
	//}
	//PLOGI << witchcraft::log_strings::game_loop_stop;

}

void cEngine::shutdown()
{
	PLOGI << witchcraft::log_strings::engine_shutdown;
	_current_engine_state = EEngineState::SHUTDOWN;


	//render_manager.shutdown();
	//PLOGI << witchcraft::log_strings::render_manager_stop;
	//
	//resource_manager.empty_cache();
	//PLOGI << witchcraft::log_strings::resource_manager_stop;
	//
	//PLOGI << witchcraft::log_strings::engine_shutdown;
}
