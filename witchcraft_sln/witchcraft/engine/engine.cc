#include "engine.h"


void Engine::startup()
{
	PLOGI << witchcraft::log_strings::engine_startup;
	current_engine_state = EEngineState::STARTUP;
	if (tm_early_exit) return;

	// project loader runs here, so we have access to our config info and save files
	{}

	// messaging layer
	{}

	// engine components
	PLOGI << witchcraft::log_strings::resource_manager_start;
	resource = std::make_unique<ResourceManager>();

	PLOGI << witchcraft::log_strings::render_manager_start;
	render = std::make_unique<RenderManager2D>();

	PLOGI << witchcraft::log_strings::scene_manager_start;
	scene = std::make_unique<SceneManager2D>();

	// this will be replaced by messaging system
	render->set_scene_manager(scene.get());
	scene->set_render_manager(render.get());
}

void Engine::run()
{
	PLOGI << witchcraft::log_strings::engine_running;
	current_engine_state = EEngineState::RUNNING;
	if (tm_early_exit) return;

	// todo: use initializer struct
	bool init_successful = render->init(
		  witchcraft::configuration::default_window_x_offset
		, witchcraft::configuration::default_window_y_offset
		, witchcraft::configuration::default_window_x_width
		, witchcraft::configuration::default_window_y_height
		, witchcraft::configuration::default_window_start_fullscreen
		, witchcraft::configuration::witchcraft_program_title.c_str()
		);
	
	if (init_successful == false)
	{
		PLOGF << witchcraft::log_strings::render_manager_init_failure << "\n" << SDL_GetError(); 
		render->shutdown();
		PLOGV << witchcraft::log_strings::render_manager_stop;
		return;
	}
	else
	{
		// the renderer initializes SDL, so this code MUST follow render init
		gameController = SDL_GameControllerOpen(0);
	}



	// - Load objects ---------------------------------------------------------------------------------

	int buddha_resource_id;
	qSceneObject * buddha_scene_object = nullptr;
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
		buddha_scene_object = render->register_render_object(render_resource);
		buddha_resource_id = id;
	}

	// pitch (field)
	int soccer_pitch_id;
	qSceneObject * soccer_pitch_scene_object = nullptr;
	{
		auto id = resource->load_from_xml_file("asset/soccer_game/stadium.asset");
		auto rr = resource->find_resource_by_id(id);
		auto render_resource = static_cast<qRenderResource*>(rr);
		render_resource->bind_renderer(render->active_renderer);
		render_resource->load();
		soccer_pitch_scene_object = render->register_render_object(render_resource);
		soccer_pitch_id = id;
	}


	// - Add objects to layers ---------------------------------------------------------------------------------

	// NOTE: this is back-to-front ordering

	// pitch (field)
	auto soccer_pitch_layer = scene->add_layer("pitch");
	soccer_pitch_layer->set_is_visible(true);
	soccer_pitch_layer->add_scene_object(static_cast<qSceneObject*>(soccer_pitch_scene_object));


	// buddha
	auto buddha_layer = scene->add_layer("buddha");
	buddha_layer->set_is_visible(true);
	buddha_layer->add_scene_object(static_cast<qSceneObject*>(buddha_scene_object));



	// - Dubug stuff ---------------------------------------------------------------------------------

	SDL_Color debug_text_color = { 0, 0, 0, 255 };
	
	std::stringstream debug_text_fps;

	// - Game Loop ---------------------------------------------------------------------------------

	bool debug_emit_frame_length = false;
	bool debug_emit_controller_count = false;
	bool debug_emit_controller_state = false;
	bool gameplay_loop_is_running = true;
	SDL_Event window_event;
	Uint32 last_frame_time = SDL_GetTicks();
	Uint32 current_frame_time = 0;

	int const controller_idx = 0;

	float player_0_x_input = 0.0f;
	float player_0_y_input = 0.0f;

	PLOGI << witchcraft::log_strings::game_loop_start;
	while (gameplay_loop_is_running)
	{
		last_frame_time = current_frame_time;
		current_frame_time = SDL_GetTicks();

		// - Event Update ---------------------------------------------------------------------------------

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

			// - Gamepad Events ------------------------------------------------------------------------

			// start button is quit
			else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
			{
				PLOGI << witchcraft::log_strings::sdl_break_event_polling;
				gameplay_loop_is_running = false;
			}

			//// 'nintendo' buttons
			//else if (window_event.caxis.which == controller_idx)
			//{
			//	if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
			//	{
			//		std::cout << "controller: [A]\n";
			//	}
			//	else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
			//	{
			//		std::cout << "controller: [B]\n";
			//	}
			//	else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
			//	{
			//		std::cout << "controller: [X]\n";
			//	}
			//	else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
			//	{
			//		std::cout << "controller: [Y]\n";
			//	}
			//}

			// controller axes
			else if (window_event.caxis.which == controller_idx)
			{
				// x-axis
				if (window_event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
				{
					// left of dead zone
					if (window_event.caxis.value < -JOYSTICK_DEAD_ZONE)
					{
						player_0_x_input = -1.f;
					}
					// right of deadzone
					else if (window_event.caxis.value > JOYSTICK_DEAD_ZONE)
					{
						player_0_x_input = 1.f;
					}
					// deadzone
					else
					{
						player_0_x_input = 0.0f;
					}
				}
				// y-axis
				else if (window_event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
				{
					// below dead zone
					if (window_event.caxis.value < -JOYSTICK_DEAD_ZONE)
					{
						player_0_y_input = -1.f;
					}
					// above dead zone
					else if (window_event.caxis.value > JOYSTICK_DEAD_ZONE)
					{
						player_0_y_input = 1.f;
					}
					// deadzone
					else
					{
						player_0_y_input = 0.0f;
					}
				}





			}

			// - Keyboard events ----------------------------------------------------------------------






			// check moar events
		}
	
		if (debug_emit_controller_state)
		{
			std::cout << "Controller[0](x,y): " << player_0_x_input << ", " << player_0_y_input << std::endl;
		}

		// - Input Update ---------------------------------------------------------------------------------

		int key_state_len = 0;
		Uint8 const * key_state = SDL_GetKeyboardState(&key_state_len);

		if (key_state[SDL_SCANCODE_W])
		{
			witchcraft::engine::move_object_by_vector(buddha_scene_object, 0, -1);
		}
		else if (key_state[SDL_SCANCODE_S]) 
		{
			witchcraft::engine::move_object_by_vector(buddha_scene_object, 0,  1);
		}
		else if (key_state[SDL_SCANCODE_A]) 
		{
			witchcraft::engine::move_object_by_vector(buddha_scene_object, -1, 0);
		}
		else if (key_state[SDL_SCANCODE_D]) 
		{
			witchcraft::engine::move_object_by_vector(buddha_scene_object, 1, 0);
		}
		else if (key_state[SDL_SCANCODE_UP])
		{
			witchcraft::engine::move_layer_by_vector(buddha_layer, 0, -1);
		}
		else if (key_state[SDL_SCANCODE_RIGHT])
		{
			witchcraft::engine::move_layer_by_vector(buddha_layer, 1, 0);
		}
		else if (key_state[SDL_SCANCODE_DOWN])
		{
			witchcraft::engine::move_layer_by_vector(buddha_layer, 0, 1);
		}
		else if (key_state[SDL_SCANCODE_LEFT])
		{
			witchcraft::engine::move_layer_by_vector(buddha_layer, -1, 0);
		}
		else if (key_state[SDL_SCANCODE_1])
		{
			debug_emit_frame_length = !debug_emit_frame_length;
		}
		else if (key_state[SDL_SCANCODE_2])
		{
			buddha_scene_object->set_position(100, 100);
		}
		else if (key_state[SDL_SCANCODE_3])
		{
			debug_emit_controller_count = true;
			gameController = witchcraft::engine::get_controller(0);
		}




		// - Physics Update ---------------------------------------------------------------------------------

		witchcraft::engine::move_object_by_vector(buddha_scene_object, player_0_x_input, player_0_y_input);


		// - Render Update ---------------------------------------------------------------------------------

		render->update();

		// - Sound Update ---------------------------------------------------------------------------------


		// - Debug ---------------------------------------------------------------------------------
		

		if (debug_emit_frame_length)
		{
			std::cout << "frame_len: " << (current_frame_time - last_frame_time) << " ms\n";
		}

		// yield for the rest of the frame
	} // !game_loop
	
	PLOGI << witchcraft::log_strings::game_loop_stop;
}

void Engine::shutdown()
{
	PLOGI << witchcraft::log_strings::engine_shutdown;
	current_engine_state = EEngineState::SHUTDOWN;
	if (tm_early_exit) return;

	// we're going to shut down SDL in the renderer, so all SDL components
	// have to be closed by then
	SDL_GameControllerClose(gameController);
	gameController = nullptr;

	render->shutdown();
	PLOGI << witchcraft::log_strings::render_manager_stop;
	
	resource->empty_cache();
	PLOGI << witchcraft::log_strings::resource_manager_stop;
	
	PLOGI << witchcraft::log_strings::engine_shutdown;
}
