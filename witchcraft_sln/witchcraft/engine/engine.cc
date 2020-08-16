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
		auto id = resource->load_from_xml_file("asset/soccer_game/images/stadium.asset");
		auto rr = resource->find_resource_by_id(id);
		auto render_resource = static_cast<qRenderResource*>(rr);
		render_resource->bind_renderer(render->active_renderer);
		render_resource->load();
		soccer_pitch_scene_object = render->register_render_object(render_resource);
		soccer_pitch_id = id;
	}

	// ball
	int ball_id;
	qSceneObject * ball_scene_object = nullptr;
	{
		auto id = resource->load_from_xml_file("asset/soccer_game/images/ball.asset");
		auto rr = resource->find_resource_by_id(id);
		auto render_resource = static_cast<qRenderResource*>(rr);
		render_resource->bind_renderer(render->active_renderer);
		render_resource->load();
		ball_scene_object = render->register_render_object(render_resource);
		ball_id = id;
	}

	// ball shadow
	int ball_shadow_id;
	qSceneObject * ball_shadow_scene_object = nullptr;
	{
		auto id = resource->load_from_xml_file("asset/soccer_game/images/ball_shadow.asset");
		auto rr = resource->find_resource_by_id(id);
		auto render_resource = static_cast<qRenderResource*>(rr);
		render_resource->bind_renderer(render->active_renderer);
		render_resource->load();
		ball_shadow_scene_object = render->register_render_object(render_resource);
		ball_shadow_id = id;
	}

	// player
	int player_id;
	qSceneObject * player_a_scene_object = nullptr;
	{
		auto id = resource->load_from_xml_file("asset/soccer_game/images/player_a.asset");
		auto rr = resource->find_resource_by_id(id);
		auto sar = static_cast<SpriteAtlasResource*>(rr);
		sar->bind_renderer(render->active_renderer);
		sar->load();
		sar->set_current_animation("idle_blink");
		player_a_scene_object = render->register_render_object(sar);
		player_id = id;
	}

	// player shadow
	int player_shadow_id;
	qSceneObject * player_shadow_scene_object = nullptr;
	{
		auto id = resource->load_from_xml_file("asset/soccer_game/images/player_shadow.asset");
		auto rr = resource->find_resource_by_id(id);
		auto render_resource = static_cast<qRenderResource*>(rr);
		render_resource->bind_renderer(render->active_renderer);
		render_resource->load();
		player_shadow_scene_object = render->register_render_object(render_resource);
		player_shadow_id = id;
	}

	// cursor
	int cursor_id;
	qSceneObject * cursor_scene_object = nullptr;
	{
		auto id = resource->load_from_xml_file("asset/soccer_game/images/cursor.asset");
		auto rr = resource->find_resource_by_id(id);
		auto render_resource = static_cast<qRenderResource*>(rr);
		render_resource->bind_renderer(render->active_renderer);
		render_resource->load();
		cursor_scene_object = render->register_render_object(render_resource);
		cursor_id = id;
	}


	// - Add objects to rendering layers -------------------------------------------------------------------------

	// NOTE: this is back-to-front ordering

	// pitch (field)
	auto soccer_pitch_layer = scene->add_layer("soccer_pitch");
	soccer_pitch_layer->set_is_visible(true);
	soccer_pitch_layer->add_scene_object(static_cast<qSceneObject*>(soccer_pitch_scene_object));


	// buddha
	auto buddha_layer = scene->add_layer("buddha");
	buddha_layer->set_is_visible(true);
	buddha_layer->add_scene_object(static_cast<qSceneObject*>(buddha_scene_object));

	// player_a
	auto player_a_layer = scene->add_layer("player_a");
	player_a_layer->set_is_visible(true);
	player_a_layer->add_scene_object(static_cast<qSceneObject*>(player_a_scene_object));



	// - Debug stuff ---------------------------------------------------------------------------------

	SDL_Color debug_text_color = { 0, 0, 0, 255 };
	
	std::stringstream debug_text_fps;

	// - Game Loop ---------------------------------------------------------------------------------

	bool debug_emit_frame_length	 = false;
	bool debug_emit_controller_count = false;
	bool debug_emit_controller_state = false;
	bool gameplay_loop_is_running	 = true;
	SDL_Event window_event;
	Uint32 last_frame_time = SDL_GetTicks();
	Uint32 current_frame_time = 0;

	int const controller_idx = 0;

	float const player_speed = 0.4f;
	float const layer_speed = 0.05f;

	float player_0_x_input = 0.0f;
	float player_0_y_input = 0.0f;
	float layer_0_x_offset = 0.0f;
	float layer_0_y_offset = 0.0f;


	PLOGI << witchcraft::log_strings::game_loop_start;
	while (gameplay_loop_is_running)
	{
		last_frame_time = current_frame_time;
		current_frame_time = SDL_GetTicks();

		// - Event Update ---------------------------------------------------------------------------------
		while (SDL_PollEvent(&window_event))
		{
			if (SDL_QUIT == window_event.type)
			{
				gameplay_loop_is_running = false;
				break;
			}

			// Keyboard events
			if (witchcraft::engine::is_keyboard_event(window_event))
			{
				if (window_event.key.type == SDL_KEYDOWN)
				{
					switch (window_event.key.keysym.sym) {
					case SDLK_ESCAPE:
						PLOGI << witchcraft::log_strings::sdl_break_event_polling;
						gameplay_loop_is_running = false;
						break;

					// WASD
					case SDLK_w:
						witchcraft::engine::move_object_by_vector(buddha_scene_object, 0.0f, -1.f);
						break;
					case SDLK_s:
						witchcraft::engine::move_object_by_vector(buddha_scene_object, 0.0f, 1.f);
						break;
					case SDLK_a:
						witchcraft::engine::move_object_by_vector(buddha_scene_object, -1.f, 0.0f);
						break;
					case SDLK_d:
						witchcraft::engine::move_object_by_vector(buddha_scene_object, 1.f, 0.0f);
						break;

					// Arrows
					case SDLK_UP:
						witchcraft::engine::move_layer_by_vector(buddha_layer, 0.0f, -1.f);
						break;
					case SDLK_RIGHT:
						witchcraft::engine::move_layer_by_vector(buddha_layer, 1.f, 0.0f);
						break;
					case SDLK_DOWN:
						witchcraft::engine::move_layer_by_vector(buddha_layer, 0.0f, 1.f);
						break;
					case SDLK_LEFT:
						witchcraft::engine::move_layer_by_vector(buddha_layer, -1.f, 0.0f);
						break;

					// Numeric
					case SDLK_1:
						debug_emit_frame_length = !debug_emit_frame_length;
						break;
					case SDLK_2:
						buddha_scene_object->set_position(100.f, 100.f);
						buddha_layer->set_offset(0.0f, 0.0f);
						break;
					case SDLK_3:
						debug_emit_controller_count = true;
						gameController = witchcraft::engine::get_controller(0);
						break;
					case SDLK_4:
						debug_emit_controller_state = !debug_emit_controller_state;
						break;
					case SDLK_5:
						break;
					case SDLK_6:
						break;
					case SDLK_7:
						break;
					case SDLK_8:
						break;
					case SDLK_9:
						break;
					case SDLK_0:
						break;
					}
				}
			} // end keyboard events

			// - Gamepad Events ------------------------------------------------------------------------
			else if (witchcraft::engine::is_gamepad_event(window_event))
			{
				// gamepad start button -> quit
				if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
				{
					PLOGI << witchcraft::log_strings::sdl_break_event_polling;
					gameplay_loop_is_running = false;
				}
				// gamepad button down
				if (window_event.cbutton.type == SDL_CONTROLLERBUTTONDOWN)
				{
					if (window_event.caxis.which == controller_idx)
					{
						// 'nintendo' buttons
						if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
						{
							std::cout << "controller: [A]\ttoggle_layer_visibility\n";
							witchcraft::engine::toggle_layer_visibility(buddha_layer);
						}
						else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
						{
							std::cout << "controller: [B]\ttoggle_layer_visibility\n";
							witchcraft::engine::toggle_layer_visibility(soccer_pitch_layer);
						}
						else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
						{
							std::cout << "controller: [X]\n";
						}
						else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
						{
							std::cout << "controller: [Y]\n";
						}

						// shoulder bumpers
						else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
						{
							auto scale = soccer_pitch_scene_object->get_scale();
							auto x = std::get<0>(scale) * 1.1f;
							auto y = std::get<1>(scale) * 1.1f;
							soccer_pitch_scene_object->set_scale(x, y);
							player_a_scene_object->set_scale(x, y);
						}
						else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
						{
							auto scale = soccer_pitch_scene_object->get_scale();
							auto x = std::get<0>(scale) * 0.9f;
							auto y = std::get<1>(scale) * 0.9f;
							soccer_pitch_scene_object->set_scale(x, y);
							player_a_scene_object->set_scale(x, y);							
						}

						// click sticks
						else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
						{
							soccer_pitch_scene_object->set_scale(1.f, 1.f);
						}
						else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
						{
							buddha_scene_object->set_scale(1.f, 1.f);
						}

					}
				}
				// gamepad axes
				else if (window_event.caxis.which == controller_idx)
				{
					// left_stick x-axis
					if (window_event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
					{
						// left of dead zone
						if (window_event.caxis.value < -JOYSTICK_DEAD_ZONE)
						{
							player_0_x_input = -player_speed;
						}
						// right of deadzone
						else if (window_event.caxis.value > JOYSTICK_DEAD_ZONE)
						{
							player_0_x_input = player_speed;
						}
						// deadzone
						else
						{
							player_0_x_input = 0.0f;
						}
					}
					// left_stick y-axis
					else if (window_event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
					{
						// below dead zone
						if (window_event.caxis.value < -JOYSTICK_DEAD_ZONE)
						{
							player_0_y_input = -player_speed;
						}
						// above dead zone
						else if (window_event.caxis.value > JOYSTICK_DEAD_ZONE)
						{
							player_0_y_input = player_speed;
						}
						// deadzone
						else
						{
							player_0_y_input = 0.0f;
						}
					}
					// right_stick x-axis
					else if (window_event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
					{
						if (window_event.caxis.value < -JOYSTICK_DEAD_ZONE)
						{
							layer_0_x_offset += -layer_speed;
						}
						else if (window_event.caxis.value > JOYSTICK_DEAD_ZONE)
						{
							layer_0_x_offset += layer_speed;
						}
						else
						{
							layer_0_x_offset = 0.0f;
						}
					}
					// right_stick y-axis
					else if (window_event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
					{
						if (window_event.caxis.value < -JOYSTICK_DEAD_ZONE)
						{
							layer_0_y_offset += -layer_speed;
						}
						else if (window_event.caxis.value > JOYSTICK_DEAD_ZONE)
						{
							layer_0_y_offset += layer_speed;
						}
						else
						{
							layer_0_y_offset = 0.0f;
						}
					}

				} // end gamepad axis events
			} // end gamepad events
		}	// end event update loop
	

		// - Physics Update ---------------------------------------------------------------------------------

		//witchcraft::engine::move_object_by_vector(buddha_scene_object, player_0_x_input, player_0_y_input);
		witchcraft::engine::move_object_by_vector(player_a_scene_object, player_0_x_input, player_0_y_input);
		witchcraft::engine::move_layer_by_vector(soccer_pitch_layer, layer_0_x_offset, layer_0_y_offset);

		// - Render Update ---------------------------------------------------------------------------------

		render->update();

		// - Sound Update ---------------------------------------------------------------------------------


		// - Debug ---------------------------------------------------------------------------------

		if (debug_emit_controller_state)
		{
			std::cout << "Controller[0](x,y): " << player_0_x_input << ", " << player_0_y_input << std::endl;
		}

		if (debug_emit_frame_length)
		{
			std::cout << "frame_len: " << (current_frame_time - last_frame_time) << " ms\n";
		}

		// yield for the rest of the frame
		//auto yield_time = witchcraft::configuration::frame_length_ms - (current_frame_time - last_frame_time);
		// HACK: only has 10ms resolution, so we're just going to use it as a 10ms sleep
		SDL_Delay(1);

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
