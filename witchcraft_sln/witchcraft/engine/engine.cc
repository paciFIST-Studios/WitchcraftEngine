#include "engine.h"


void cEngine::startup()
{
	PLOGI << witchcraft::log_strings::engine_startup;
	_current_engine_state = EEngineState::STARTUP;
	if (tm_early_exit) return;


	// messaging layer
	// todo

	PLOGI << witchcraft::log_strings::resource_manager_start;
	resource = std::make_unique<qResourceManager>();

	PLOGI << witchcraft::log_strings::render_manager_start;
	render = std::make_unique<q2DRenderManager>();
}

void cEngine::run()
{
	PLOGI << witchcraft::log_strings::engine_running;
	_current_engine_state = EEngineState::RUNNING;
	if (tm_early_exit) return;


	// todo: use initializer struct
	bool init_successful = render->init(
		  0			// x offset
		, 0			// y offset
		, 800		// x size - width
		, 800		// y size - height
		, false		// fullscreen
		, witchcraft::configuration::witchcraft_program_title.c_str()
		);
	

	if (init_successful == false)
	{
		PLOGF << witchcraft::log_strings::render_manager_init_failure << "\n" << SDL_GetError(); 
		render->shutdown();
		PLOGV << witchcraft::log_strings::render_manager_stop;
		return;
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
		render->update();
	
		// do sound update
	}
	
	PLOGI << witchcraft::log_strings::game_loop_stop;
}

void cEngine::shutdown()
{
	PLOGI << witchcraft::log_strings::engine_shutdown;
	_current_engine_state = EEngineState::SHUTDOWN;
	if (tm_early_exit) return;


	render->shutdown();
	PLOGI << witchcraft::log_strings::render_manager_stop;
	
	resource->empty_cache();
	PLOGI << witchcraft::log_strings::resource_manager_stop;
	
	PLOGI << witchcraft::log_strings::engine_shutdown;
}
