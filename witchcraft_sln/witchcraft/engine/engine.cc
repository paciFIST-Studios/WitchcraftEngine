#include "engine.h"

void Engine::init_system()
{
	PLOGI << witchcraft::log_strings::engine_startup;
	current_engine_state = EEngineState::STARTUP;
	if (test_mode.early_exit) return;

	// initialize sdl ----------------------------------------------------------------------------------
	// the audio component and render component both use SDL, so basic init preceeds both
	PLOGI << witchcraft::log_strings::sdl_start;
	if (init_sdl() < 0) {
		PLOGF << witchcraft::log_strings::sdl_init_failure << "\nError: " << SDL_GetError();
		return;
	}
	PLOGV << witchcraft::log_strings::sdl_ok;

	// engine components ------------------------------------------------------------------------------------------
	PLOGI << witchcraft::log_strings::message_bus_start;
	message = std::make_unique<MessageBus>();
	{
		std::function<void(Message)> cb = std::bind(&Engine::handle_message, this, std::placeholders::_1);
		message->subscribe("engine", cb);
	}
	PLOGI << witchcraft::log_strings::message_bus_ok;

	PLOGI << witchcraft::log_strings::resource_manager_start;
	resource = std::make_unique<ResourceManager>(message.get());
	PLOGI << witchcraft::log_strings::resource_manager_ok;

	PLOGI << witchcraft::log_strings::audio_manager_start;
	audio = std::make_unique<AudioManager>(message.get());
	PLOGI << witchcraft::log_strings::audio_manager_ok;

	PLOGI << witchcraft::log_strings::debug_console_start;
	console = std::make_unique<Console>(message.get());
	PLOGI << witchcraft::log_strings::debug_console_ok;

	PLOGI << witchcraft::log_strings::render_manager_start;
	render = std::make_unique<RenderManager>(message.get());
	PLOGI << witchcraft::log_strings::render_manager_ok;

	PLOGI << witchcraft::log_strings::scene_manager_start;
	scene = std::make_unique<SceneManager2D>(message.get());
	PLOGI << witchcraft::log_strings::scene_manager_ok;

	// game components ------------------------------------------------------------------------------------------

	// project loader runs once, and then it's done.  If we need more
	// things to happen, we can tie it in to the message bus, but
	// we'll save that until some project requires it
	PLOGI << witchcraft::log_strings::project_loader_start;
	project_loader = std::make_unique<ProjectLoader>(project_file_path);
	PLOGI << witchcraft::log_strings::project_loader_ok;

	PLOGI << witchcraft::log_strings::gameplay_manager_start;
	gameplay = std::make_unique<GameplayManager>(message.get());
	PLOGI << "gameplay manager ok";

	PLOGI << witchcraft::log_strings::engine_ok;
}

int Engine::init_sdl()
{
	// SDL_INIT_EVENTS, is initialized by joystick, and video

	/* -1 == error, 0 == success */
	return SDL_Init(
		  SDL_INIT_AUDIO
		| SDL_INIT_VIDEO
		//| SDL_INIT_HAPTIC
		//| SDL_INIT_GAMECONTROLLER
	);
}


void Engine::final_engine_component_initialization()
{
	// frequently used channel ids
	audio_channel_id    = message->channel_lookup("audio"   );
	console_channel_id  = message->channel_lookup("console" );
	engine_channel_id   = message->channel_lookup("engine"  );
	render_channel_id   = message->channel_lookup("render"  );
	resource_channel_id = message->channel_lookup("resource");
	scene_channel_id    = message->channel_lookup("scene"   );

	// default asset loading
	{
		// for now, this is a default engine resource
		resource->load_from_xml_file("asset/textured_quad.asset");
		resource->load_from_xml_file("asset/basic_shader.asset");
		resource->load_from_xml_file("asset/buddha.asset");
	}
	
	project_loader->parse_project_file();
	current_project = project_loader->get_project_settings();

	resource->init_component();
	audio->init_component();
	console->init_component();
	// scene->init_component();
	project_loader->init_component();
	gameplay->init_component();
	
	render->init_system(
		  current_project.window_x
		, current_project.window_y
		, current_project.window_w
		, current_project.window_h
		, current_project.use_fullscreen
		, current_project.window_title.c_str()
	);

	// the renderer initializes SDL, so this code MUST follow render init
	gameController = SDL_GameControllerOpen(0);
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
			case SDLK_F5:
				send_render_command("use_texture_class=toggle");
				break;
			case SDLK_F6:
				send_render_command("use_vertex_class_not_quad=toggle");
			case SDLK_F7:
				send_render_command("use_shader=basic");
				break;
			case SDLK_F8:
				send_render_command("use_shader=textureless");
				break;


			// Numeric
			case SDLK_1:
				send_audio_message("asset/soccer_game/sounds/cakeflaps_quinklette__16bit.wav", MessageType::REQUEST__AUDIO_LOAD);
				break;
			case SDLK_2:
				send_audio_message("asset/soccer_game/sounds/cakeflaps_quinklette__16bit.wav", MessageType::REQUEST__AUDIO_PLAY);
				break;
			case SDLK_3:
				break;
			case SDLK_4:
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
	
}

void Engine::send_console_command(char const * command, bool send_direct)
{
	direct_message_string_buffer = std::string(command);
	send_message(
		  console_channel_id
		, engine_channel_id
		, MessageType::INVOKE__CONSOLE_COMMAND
		, static_cast<void*>(&direct_message_string_buffer)
		, send_direct
	);
}

void Engine::send_render_command(char const * command, bool send_direct)
{
	direct_message_string_buffer = std::string(command);
	send_message(
		  render_channel_id
		, engine_channel_id
		, MessageType::INVOKE__RENDER_COMMAND
		, static_cast<void*>(&direct_message_string_buffer)
		, send_direct
	);
}

void Engine::send_audio_message(char const * path, MessageType type, bool send_direct)
{
	direct_message_string_buffer = std::string(path);
	send_message(
		  audio_channel_id
		, engine_channel_id
		, type
		, static_cast<void*>(&direct_message_string_buffer)
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

	
	final_engine_component_initialization();


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
	//
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
	

	// - Add objects to rendering layers -------------------------------------------------------------------------

	// NOTE: this is back-to-front ordering

	// buddha
	//auto buddha_layer = scene->add_layer("buddha");
	//buddha_layer->set_is_visible(true);
	//buddha_layer->add_scene_object(static_cast<qSceneObject*>(buddha_scene_object));

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
	float const layer_speed  = 0.05f;
	float player_0_x_input   = 0.0f;
	float player_0_y_input   = 0.0f;
	float layer_0_x_offset   = 0.0f;
	float layer_0_y_offset   = 0.0f;
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

	} // !game_loop

	PLOGI << witchcraft::log_strings::game_loop_stop;
}

void Engine::shutdown()
{
	PLOGI << witchcraft::log_strings::engine_shutdown;
	current_engine_state = EEngineState::SHUTDOWN;
	if (test_mode.early_exit) { return; }

	PLOGI << "[memory] total alloc before shutdown: " << witchcraft::memory::allocation_tracker.total_allocated;
	PLOGI << "[memory] total free before shutdown:  " << witchcraft::memory::allocation_tracker.total_freed;
	PLOGI << "[memory] active use before shutdown:  " << witchcraft::memory::allocation_tracker.current_usage();

	auto stats = render->get_window_stats();
	current_project.window_x = stats.x;
	current_project.window_y = stats.y;
	current_project.window_h = stats.h;
	current_project.window_w = stats.w;
	current_project.use_fullscreen = (stats.flags & SDL_WINDOW_FULLSCREEN);
	current_project.capture_mouse  = (stats.flags & SDL_WINDOW_MOUSE_CAPTURE);
	project_loader->update_and_save(current_project);

	// we're going to shut down SDL in the renderer, so all SDL components
	// have to be closed by then
	SDL_GameControllerClose(gameController);
	gameController = nullptr;

	audio->shutdown();
	PLOGI << witchcraft::log_strings::audio_manager_stop;

	render->shutdown();
	PLOGI << witchcraft::log_strings::render_manager_stop;

	resource->empty_cache();
	PLOGI << witchcraft::log_strings::resource_manager_stop;

	PLOGI << witchcraft::log_strings::engine_shutdown;	
}
