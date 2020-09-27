#include "engine.h"


void Engine::startup()
{
	PLOGI << witchcraft::log_strings::engine_startup;
	current_engine_state = EEngineState::STARTUP;
	if (test_mode.early_exit) return;

	// engine components ------------------------------------------------------------------------------------------
	PLOGI << witchcraft::log_strings::message_bus_start;
	message = std::make_unique<MessageBus>();
	{
		std::function<void(Message)> cb = std::bind(&Engine::handle_message, this, std::placeholders::_1);
		message->subscribe("engine", cb);
		engine_channel_id	= message->channel_lookup("engine"	);
		resource_channel_id = message->channel_lookup("resource");
		render_channel_id	= message->channel_lookup("render"	);
		scene_channel_id	= message->channel_lookup("scene"	);
		console_channel_id	= message->channel_lookup("console"	);
	}

	PLOGI << witchcraft::log_strings::resource_manager_start;
	resource = std::make_unique<ResourceManager>(message.get());
	{
		// for now, this is a default engine resource
		resource->load_from_xml_file("asset/textured_quad.asset");
	}

	PLOGI << witchcraft::log_strings::debug_console;
	console = std::make_unique<Console>(message.get());

	PLOGI << witchcraft::log_strings::render_manager_start;
	render = std::make_unique<RenderManager>(message.get());

	PLOGI << witchcraft::log_strings::scene_manager_start;
	scene = std::make_unique<SceneManager2D>(message.get());

	// game components ------------------------------------------------------------------------------------------

	// project loader runs once, and then it's done.  If we need more
	// things to happen, we can tie it in to the message bus, but
	// we'll save that until some project requires it
	PLOGI << witchcraft::log_strings::project_loader_start;
	project_loader = std::make_unique<ProjectLoader>(project_file_path);
	project_loader->parse_project_file();
	project_settings = project_loader->get_project_settings();

	init_gameplay(project_settings);
}

void Engine::init_gameplay(ProjectSettings ps)
{
	PLOGI << witchcraft::log_strings::gameplay_manager_start;
	gameplay = std::make_unique<GameplayManager>(message.get());

	for (auto&& path : ps.file_paths)
	{
		resource->load_from_xml_file(path);
	}
}

bool Engine::continue_gameplay_loop(SDL_Event const & e)
{
	if (   SDL_QUIT    == e.type
	    || SDLK_ESCAPE == e.key.keysym.sym)
	{
		//PLOGI << witchcraft::log_strings::sdl_break_event_polling;
		return false;
	}

	return true;
}

void Engine::process_window_event(SDL_Event const & e)
{
	// Keyboard events
	if (witchcraft::engine::is_keyboard_event(e))
	{
		if (e.key.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym) {

				// WASD
				//case SDLK_w:
				//	witchcraft::engine::move_object_by_vector(buddha_scene_object, 0.0f, -1.f);
				//	break;
				//case SDLK_s:
				//	witchcraft::engine::move_object_by_vector(buddha_scene_object, 0.0f, 1.f);
				//	break;
				//case SDLK_a:
				//	witchcraft::engine::move_object_by_vector(buddha_scene_object, -1.f, 0.0f);
				//	break;
				//case SDLK_d:
				//	witchcraft::engine::move_object_by_vector(buddha_scene_object, 1.f, 0.0f);
				//	break;

				//// Arrows
				//case SDLK_UP:
				//	witchcraft::engine::move_layer_by_vector(buddha_layer, 0.0f, -1.f);
				//	break;
				//case SDLK_RIGHT:
				//	witchcraft::engine::move_layer_by_vector(buddha_layer, 1.f, 0.0f);
				//	break;
				//case SDLK_DOWN:
				//	witchcraft::engine::move_layer_by_vector(buddha_layer, 0.0f, 1.f);
				//	break;
				//case SDLK_LEFT:
				//	witchcraft::engine::move_layer_by_vector(buddha_layer, -1.f, 0.0f);
				//	break;

			case SDLK_F1:
				render->toggle_imgui_debug_window();
				//send_command(render, "debug_window_is_visible=true")
				break;
			case SDLK_F2:
				send_console_command("draw_console=toggle");
				break;
			case SDLK_F3:
				send_render_command("render_wireframe=toggle");
				break;
			case SDLK_F4:
				send_render_command("triangle2quad=toggle");
				break;

				// Numeric
			case SDLK_1:
				//debug.emit_frame_length = !debug.emit_frame_length;
				break;
				//case SDLK_2:
				//	buddha_scene_object->set_position(100.f, 100.f);
				//	buddha_layer->set_offset(0.0f, 0.0f);
				//	break;
			case SDLK_3:
				//debug.emit_controller_count = true;
				gameController = witchcraft::engine::get_controller(0);
				break;
			case SDLK_4:
				//debug.emit_controller_state = !debug.emit_controller_state;
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
	//else if (witchcraft::engine::is_gamepad_event(window_event))
	//{
	//	// gamepad start button -> quit
	//	if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
	//	{
	//		PLOGI << witchcraft::log_strings::sdl_break_event_polling;
	//		gameplay_loop_is_running = false;
	//	}
	//	// gamepad button down
	//	if (window_event.cbutton.type == SDL_CONTROLLERBUTTONDOWN)
	//	{
	//		if (window_event.caxis.which == controller_idx)
	//		{
	//			//// 'nintendo' buttons
	//			//if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
	//			//{
	//			//	std::cout << "controller: [A]\ttoggle_layer_visibility\n";
	//			//	witchcraft::engine::toggle_layer_visibility(buddha_layer);
	//			//}
	//			//else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
	//			//{
	//			//	std::cout << "controller: [B]\ttoggle_layer_visibility\n";
	//			//	witchcraft::engine::toggle_layer_visibility(soccer_pitch_layer);
	//			//}
	//			//else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
	//			//{
	//			//	std::cout << "controller: [X]\n";
	//			//}
	//			//else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
	//			//{
	//			//	std::cout << "controller: [Y]\n";
	//			//}
	//			//
	//			//// shoulder bumpers
	//			//else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
	//			//{
	//			//	auto scale = soccer_pitch_scene_object->get_scale();
	//			//	auto x = std::get<0>(scale) * 1.1f;
	//			//	auto y = std::get<1>(scale) * 1.1f;
	//			//	soccer_pitch_scene_object->set_scale(x, y);
	//			//}
	//			//else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
	//			//{
	//			//	auto scale = soccer_pitch_scene_object->get_scale();
	//			//	auto x = std::get<0>(scale) * 0.9f;
	//			//	auto y = std::get<1>(scale) * 0.9f;
	//			//	soccer_pitch_scene_object->set_scale(x, y);
	//			//}
	//			//
	//			//// click sticks
	//			//else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
	//			//{
	//			//	soccer_pitch_scene_object->set_scale(1.f, 1.f);
	//			//}
	//			//else if (window_event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
	//			//{
	//			//	buddha_scene_object->set_scale(1.f, 1.f);
	//			//}
	//		}
	//	}
	//	// gamepad axes
	//	else if (window_event.caxis.which == controller_idx)
	//	{
	//		// left_stick x-axis
	//		if (window_event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
	//		{
	//			// left of dead zone
	//			if (window_event.caxis.value < -JOYSTICK_DEAD_ZONE)
	//			{
	//				player_0_x_input = -player_speed;
	//			}
	//			// right of deadzone
	//			else if (window_event.caxis.value > JOYSTICK_DEAD_ZONE)
	//			{
	//				player_0_x_input = player_speed;
	//			}
	//			// deadzone
	//			else
	//			{
	//				player_0_x_input = 0.0f;
	//			}
	//		}
	//		// left_stick y-axis
	//		else if (window_event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
	//		{
	//			// below dead zone
	//			if (window_event.caxis.value < -JOYSTICK_DEAD_ZONE)
	//			{
	//				player_0_y_input = -player_speed;
	//			}
	//			// above dead zone
	//			else if (window_event.caxis.value > JOYSTICK_DEAD_ZONE)
	//			{
	//				player_0_y_input = player_speed;
	//			}
	//			// deadzone
	//			else
	//			{
	//				player_0_y_input = 0.0f;
	//			}
	//		}
	//		// right_stick x-axis
	//		else if (window_event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
	//		{
	//			if (window_event.caxis.value < -JOYSTICK_DEAD_ZONE)
	//			{
	//				layer_0_x_offset += -layer_speed;
	//			}
	//			else if (window_event.caxis.value > JOYSTICK_DEAD_ZONE)
	//			{
	//				layer_0_x_offset += layer_speed;
	//			}
	//			else
	//			{
	//				layer_0_x_offset = 0.0f;
	//			}
	//		}
	//		// right_stick y-axis
	//		else if (window_event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
	//		{
	//			if (window_event.caxis.value < -JOYSTICK_DEAD_ZONE)
	//			{
	//				layer_0_y_offset += -layer_speed;
	//			}
	//			else if (window_event.caxis.value > JOYSTICK_DEAD_ZONE)
	//			{
	//				layer_0_y_offset += layer_speed;
	//			}
	//			else
	//			{
	//				layer_0_y_offset = 0.0f;
	//			}
	//		}
	//
	//	} // end gamepad axis events
	//} // end gamepad events

}

void Engine::send_console_command(char const * command, bool send_direct)
{
	string_buffer = std::string(command);
	send_message(
		  console_channel_id
		, engine_channel_id
		, MessageType::INVOKE__CONSOLE_COMMAND
		, static_cast<void*>(&string_buffer) 
		, send_direct
	);
}

void Engine::send_render_command(char const * command, bool send_direct)
{
	string_buffer = std::string(command);
	send_message(
		  render_channel_id
		, engine_channel_id
		, MessageType::INVOKE__RENDER_COMMAND
		, static_cast<void*>(&string_buffer)
		, send_direct
	);
}

void Engine::send_message(unsigned int sendTo, unsigned int sendFrom, MessageType type, void * data, bool send_direct)
{
	Message m { sendTo, sendFrom, type, data };
	if (send_direct)
	{
		message->send_direct_message(m);
	}
	else
	{
		message->send_message(m);
	}
}

void Engine::handle_message(Message m)
{
	if (m.type == MessageType::REQUEST__CONSOLE_PTR_NON_OWNER)
	{
		// to, from, type, data
		send_message(
			  m.sender
			, engine_channel_id
			, MessageType::SUPPLY__CONSOLE_PTR_NON_OWNER
			, console.get()
		);
	}
}



void Engine::run()
{
	PLOGI << witchcraft::log_strings::engine_running;
	current_engine_state = EEngineState::RUNNING;
	if (test_mode.early_exit) return;

	bool init_successful = render->init_system(
		  witchcraft::configuration::default_window_x_offset
		, witchcraft::configuration::default_window_y_offset
		, witchcraft::configuration::default_window_x_width
		, witchcraft::configuration::default_window_y_height
		, witchcraft::configuration::default_window_start_fullscreen
		, witchcraft::configuration::witchcraft_program_title
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

	// steps:
	// get paths from project file
	// load all paths with resource manager
	// if paths should be in a specific order, then order them in the manifest
	// return some kind of package, containing pointers to the objects
	//		asset objects are still owned by the resource manager
	// give package to gameplay logic
	// gameplay logic acts upon package of asset objects
	// gameplay handles inputs, modifying positions, modifying
	//		options
	// when gameplay finishes, system begins shutdown
	// since asset objects are owned by resource manager, they get torn down
	//		when resource manager is deconstructed

	//int buddha_resource_id;
	//qSceneObject * buddha_scene_object = nullptr;
	//// once the project loader exists, we can load files, based on what it says.
	//// loop over a set of strings, which are our asset paths
	//{
	//	// In this iteration of the game, we set up the scene in this method,
	//	// and we load all the assets we'll ever use in the game, right now.
	//	// any final setup work should happen here
	//
	//	auto rr = resource->load_from_xml_file(witchcraft::configuration::buddha_asset);
	//	auto render_resource = static_cast<SDLRenderResource*>(rr);
	//	render_resource->bind_renderer(render->get_active_renderer());
	//	render_resource->load();
	//	buddha_scene_object = render->register_render_object(render_resource);
	//	buddha_resource_id = rr->id;
	//}

	//// pitch (field)
	//int soccer_pitch_id;
	//qSceneObject * soccer_pitch_scene_object = nullptr;
	//{
	//	auto id = resource->load_from_xml_file("asset/soccer_game/images/stadium.asset");
	//	auto rr = resource->find_resource_by_id(id);
	//	auto render_resource = static_cast<SDLRenderResource*>(rr);
	//	render_resource->bind_renderer(render->active_renderer);
	//	render_resource->load();
	//	soccer_pitch_scene_object = render->register_render_object(render_resource);
	//	soccer_pitch_id = id;
	//}
	//
	//// ball
	//int ball_id;
	//qSceneObject * ball_scene_object = nullptr;
	//{
	//	auto id = resource->load_from_xml_file("asset/soccer_game/images/ball.asset");
	//	auto rr = resource->find_resource_by_id(id);
	//	auto render_resource = static_cast<SDLRenderResource*>(rr);
	//	render_resource->bind_renderer(render->active_renderer);
	//	render_resource->load();
	//	ball_scene_object = render->register_render_object(render_resource);
	//	ball_id = id;
	//}
	//
	//// ball shadow
	//int ball_shadow_id;
	//qSceneObject * ball_shadow_scene_object = nullptr;
	//{
	//	auto id = resource->load_from_xml_file("asset/soccer_game/images/ball_shadow.asset");
	//	auto rr = resource->find_resource_by_id(id);
	//	auto render_resource = static_cast<SDLRenderResource*>(rr);
	//	render_resource->bind_renderer(render->active_renderer);
	//	render_resource->load();
	//	ball_shadow_scene_object = render->register_render_object(render_resource);
	//	ball_shadow_id = id;
	//}
	//
	//// player
	//int player_id;
	//qSceneObject * player_a_scene_object = nullptr;
	//{
	//	auto id = resource->load_from_xml_file("asset/soccer_game/images/player_a.asset");
	//	auto rr = resource->find_resource_by_id(id);
	//	auto sar = static_cast<SpriteAtlasResource*>(rr);
	//	sar->bind_renderer(render->active_renderer);
	//	sar->load();
	//	sar->set_current_animation("idle_blink");
	//	player_a_scene_object = render->register_render_object(sar);
	//	player_id = id;
	//}
	//
	//// player shadow
	//int player_shadow_id;
	//qSceneObject * player_shadow_scene_object = nullptr;
	//{
	//	auto id = resource->load_from_xml_file("asset/soccer_game/images/player_shadow.asset");
	//	auto rr = resource->find_resource_by_id(id);
	//	auto render_resource = static_cast<SDLRenderResource*>(rr);
	//	render_resource->bind_renderer(render->active_renderer);
	//	render_resource->load();
	//	player_shadow_scene_object = render->register_render_object(render_resource);
	//	player_shadow_id = id;
	//}
	//
	//// cursor
	//int cursor_id;
	//qSceneObject * cursor_scene_object = nullptr;
	//{
	//	auto id = resource->load_from_xml_file("asset/soccer_game/images/cursor.asset");
	//	auto rr = resource->find_resource_by_id(id);
	//	auto render_resource = static_cast<SDLRenderResource*>(rr);
	//	render_resource->bind_renderer(render->active_renderer);
	//	render_resource->load();
	//	cursor_scene_object = render->register_render_object(render_resource);
	//	cursor_id = id;
	//}


	// - Add objects to rendering layers -------------------------------------------------------------------------

	// NOTE: this is back-to-front ordering

	//// pitch (field)
	//auto soccer_pitch_layer = scene->add_layer("soccer_pitch");
	//soccer_pitch_layer->set_is_visible(true);
	//soccer_pitch_layer->add_scene_object(static_cast<qSceneObject*>(soccer_pitch_scene_object));
	//
	// buddha
	//auto buddha_layer = scene->add_layer("buddha");
	//buddha_layer->set_is_visible(true);
	//buddha_layer->add_scene_object(static_cast<qSceneObject*>(buddha_scene_object));
	
	//// player_a
	//auto player_a_layer = scene->add_layer("player_a");
	//player_a_layer->set_is_visible(true);
	//player_a_layer->add_scene_object(static_cast<qSceneObject*>(player_a_scene_object));



	// - Debug stuff ---------------------------------------------------------------------------------

	SDL_Color debug_text_color = { 0, 0, 0, 255 };
	
	std::stringstream debug_text_fps;

	// - Game Loop ---------------------------------------------------------------------------------

	DebugOptions debug{
		  false	// emit frame len
		, false	// emit controller count
		, false	// emit controller state
	};

	bool gameplay_loop_is_running	 = true;
	SDL_Event window_event;
	Uint32 last_frame_time = SDL_GetTicks();
	Uint32 current_frame_time = 0;

	int const controller_idx = 0;

	// owned by gameplay object
	float const player_speed = 0.4f;
	float const layer_speed = 0.05f;
	float player_0_x_input = 0.0f;
	float player_0_y_input = 0.0f;
	float layer_0_x_offset = 0.0f;
	float layer_0_y_offset = 0.0f;
	// ! owned by gameplay object

	PLOGI << witchcraft::log_strings::game_loop_start;
	while (gameplay_loop_is_running)
	{
		last_frame_time = current_frame_time;
		current_frame_time = SDL_GetTicks();

		// - Event Update ---------------------------------------------------------------------------------
		while (SDL_PollEvent(&window_event))
		{
			gameplay_loop_is_running = continue_gameplay_loop(window_event);
			ImGui_ImplSDL2_ProcessEvent(&window_event);
			process_window_event(window_event);
		}
	
		// - Physics Update ---------------------------------------------------------------------------------

		//witchcraft::engine::move_object_by_vector(buddha_scene_object, player_0_x_input, player_0_y_input);
		//witchcraft::engine::move_object_by_vector(player_a_scene_object, player_0_x_input, player_0_y_input);
		//witchcraft::engine::move_layer_by_vector(soccer_pitch_layer, layer_0_x_offset, layer_0_y_offset);

		// - Render Update ---------------------------------------------------------------------------------

		render->update();


		// - Sound Update ---------------------------------------------------------------------------------


		// - Debug ---------------------------------------------------------------------------------

		if (debug.emit_controller_state)
		{
			std::cout << "Controller[0](x,y): " << player_0_x_input << ", " << player_0_y_input << std::endl;
		}

		if (debug.emit_frame_length)
		{
			std::cout << "frame_len: " << (current_frame_time - last_frame_time) << " ms\n";
		}

		// yield for the rest of the frame
		//auto yield_time = witchcraft::configuration::frame_length_ms - (current_frame_time - last_frame_time);
		// HACK: only has 10ms resolution, so we're just going to use it as a 10ms sleep
		//SDL_Delay(1);

	} // !game_loop
	
	PLOGI << witchcraft::log_strings::game_loop_stop;
}

void Engine::shutdown()
{
	PLOGI << witchcraft::log_strings::engine_shutdown;
	current_engine_state = EEngineState::SHUTDOWN;
	if (test_mode.early_exit) return;

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
