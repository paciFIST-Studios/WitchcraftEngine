#include "engine.h"


void Engine::startup()
{
	PLOGI << witchcraft::log_strings::engine_startup;
	current_engine_state = EEngineState::STARTUP;
	if (tm_early_exit) return;

	// project loader runs here, so we have access to our config info and save files


	// messaging layer
	// todo

	PLOGI << witchcraft::log_strings::resource_manager_start;
	resource = std::make_unique<qResourceManager>();

	PLOGI << witchcraft::log_strings::render_manager_start;
	render = std::make_unique<q2DRenderManager>();
}

void Engine::run()
{
	PLOGI << witchcraft::log_strings::engine_running;
	current_engine_state = EEngineState::RUNNING;
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
	
	// ----------------------------------------------------------------------------------

	int buddha_resource_id;
	// once the project loader exists, we can load files, based on what it says.
	// loop over a set of strings, which are our asset paths
	{
		// In this iteration of the game, we set up the scene in this method,
		// and we load all the assets we'll ever use in the game, right now.
		// any final setup work should happen here

		auto id = resource->load_from_xml_file(witchcraft::configuration::buddha_asset);
		auto rr = resource->find_resource_by_id(id);
		auto render_resource = static_cast<qRenderResource*>(rr);
		render_resource->bind_renderer(render->active_renderer);
		render_resource->load();
		render->register_render_object(render_resource);
		buddha_resource_id = id;
	}

	int const move_speed = 20;
	auto buddha = render->get_render_object(buddha_resource_id);
	
	// ----------------------------------------------------------------------------------

	SDL_Color debug_text_color = { 0, 0, 0, 255 };
	
	std::stringstream debug_text_fps;

	// ----------------------------------------------------------------------------------


	bool debug_emit_frame_length = false;
	bool gameplay_loop_is_running = true;
	SDL_Event window_event;
	Uint32 last_frame_time = SDL_GetTicks();
	Uint32 current_frame_time = 0;

	PLOGI << witchcraft::log_strings::game_loop_start;
	while (gameplay_loop_is_running)
	{
		last_frame_time = current_frame_time;
		current_frame_time = SDL_GetTicks();

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
				else if (window_event.key.keysym.sym == SDLK_w)
				{
					auto wh = buddha->get_position();
					auto x = int(std::get<0>(wh));
					auto y = int(std::get<1>(wh));
					y -= move_speed;
					buddha->set_position(static_cast<float>(x), static_cast<float>(y));
				}
				else if (window_event.key.keysym.sym == SDLK_a)
				{
					auto wh = buddha->get_position();
					auto x = int(std::get<0>(wh));
					auto y = int(std::get<1>(wh));
					x -= move_speed;
					buddha->set_position(static_cast<float>(x), static_cast<float>(y));
				}
				else if (window_event.key.keysym.sym == SDLK_s)
				{
					auto wh = buddha->get_position();
					auto x = int(std::get<0>(wh));
					auto y = int(std::get<1>(wh));
					y += move_speed;
					buddha->set_position(static_cast<float>(x), static_cast<float>(y));

				}
				else if (window_event.key.keysym.sym == SDLK_d)
				{
					auto wh = buddha->get_position();
					auto x = int(std::get<0>(wh));
					auto y = int(std::get<1>(wh));
					x += move_speed;
					buddha->set_position(static_cast<float>(x), static_cast<float>(y));
				}
				else if (window_event.key.keysym.sym == SDLK_1)
				{
					// toggle emit_frame_len
					debug_emit_frame_length = !debug_emit_frame_length;
				}
			}	
			// check moar events
		}
	
		// do physics update
	
		// do render update
		render->update();
	
		// do sound update
		
		if (debug_emit_frame_length)
		{
			std::cout << "frame_len: " << (current_frame_time - last_frame_time) << " ms\n";
		}

	}
	
	PLOGI << witchcraft::log_strings::game_loop_stop;
}

void Engine::shutdown()
{
	PLOGI << witchcraft::log_strings::engine_shutdown;
	current_engine_state = EEngineState::SHUTDOWN;
	if (tm_early_exit) return;


	render->shutdown();
	PLOGI << witchcraft::log_strings::render_manager_stop;
	
	resource->empty_cache();
	PLOGI << witchcraft::log_strings::resource_manager_stop;
	
	PLOGI << witchcraft::log_strings::engine_shutdown;
}
