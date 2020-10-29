#include "render_manager.h"

void RenderManager::init_system(unsigned xOffset, unsigned yOffset, unsigned Width, unsigned Height, bool fullScreen, char const * WindowTitle)
{
	PLOGI << witchcraft::log_strings::render_manager_system_init_start;

	bool abort = false;

	if ( ! init_sdl_window(xOffset, yOffset, Width, Height, WindowTitle))
	{ abort = true; }
	//if ( ! init_sdl_image()){ abort = true; }
	
	if ( ! init_opengl())	{ abort = true; }

	// NOTE: this initialization has to come AFTER the opengl init
	if ( ! init_imgui())	{ abort = true; }

	// init shaders
	if ( ! init_shaders())	{ abort = true; }
	if ( ! init_geometry())	{ abort = true; }

	if (abort)
	{
		PLOGF << witchcraft::log_strings::render_manager_init_failure << "\n" << SDL_GetError();
		this->shutdown();
		PLOGV << witchcraft::log_strings::render_manager_stop;
		return;
	}

	request_debug_console_ptr();

	PLOGI << witchcraft::log_strings::render_manager_system_init_end;
	PLOGI << witchcraft::log_strings::render_manager_ok;

	renderer_state = ERendererState::UPDATE_OK;
}

bool RenderManager::init_sdl_window(unsigned xOffset, unsigned yOffset, unsigned Width, unsigned Height, char const * WindowTitle)
{
	PLOGV << witchcraft::log_strings::sdl_window_create;
	program_window = SDL_CreateWindow(
		  WindowTitle
		, xOffset
		, yOffset
		, Width	
		, Height
		, SDL_WINDOW_OPENGL
	);
	if (program_window == nullptr)
	{
		PLOGF << witchcraft::log_strings::sdl_window_fail;
		return false;
	}

	PLOGV << witchcraft::log_strings::sdl_window_ok;	
	renderer_state = ERendererState::SDL_INIT_OK;
	return true;
}

bool RenderManager::init_opengl()
{
	PLOGI << witchcraft::log_strings::opengl_context_create;

	// OpenGL 3.3 rendering context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// mac osx
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// context is automatically attached to window here
	opengl_context = SDL_GL_CreateContext(program_window);
	if (opengl_context == nullptr)
	{
		PLOGF << "FAILURE: opengl context: could not create context!";
		SDL_DestroyWindow(program_window);
		program_window = nullptr;
		SDL_Quit();
		return false;
	}
	else
	{
		PLOGV << witchcraft::log_strings::opengl_context_ok;
	}

	// not technically part of opengl, but we'll initialize it here, b/c it 
	// won't relate to anything other than opengl, in our system

	// glew
	PLOGV << witchcraft::log_strings::opengl_glew_start;
	glewExperimental = GL_TRUE;
	auto error = glewInit();
	if (error != GLEW_OK)
	{
		PLOGF << witchcraft::log_strings::opengl_glew_init_failure;
		SDL_GL_DeleteContext(opengl_context);
		SDL_DestroyWindow(program_window);
		program_window = nullptr;
		SDL_Quit();
		return false;
	}
	else
	{
		PLOGV << witchcraft::log_strings::opengl_glew_ok;
		PLOGV << "gfx card info: "  << glGetString(GL_RENDERER);
		PLOGV << "opengl version: " << glGetString(GL_VERSION);
	}

	scc = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);

	// blending fn for showing transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//SDL_GL_SetSwapInterval(1);	// use VSYNC
	glEnable(GL_DEPTH_TEST);	// only draw closest pixel to screen
	glDepthFunc(GL_LESS);		// for depth test, smaller == closer

	// our built in billboard
	sprite_texture.load();
	billboard_object.set_texture(sprite_texture);

	renderer_state = ERendererState::OPENGL_INIT_OK;

	return true;
}


bool RenderManager::init_imgui()
{
	// NOTE: these early-out returns are basically
	// only present for style, several of these fns never
	// return false

	IMGUI_CHECKVERSION();
	
	auto imgui_context = ImGui::CreateContext();
	if (imgui_context == nullptr){return false;}

	ImGuiIO &io = ImGui::GetIO();
	
	auto sdl_init = ImGui_ImplSDL2_InitForOpenGL(program_window, opengl_context);
	if (sdl_init == false){return false;}

	auto opengl_init = ImGui_ImplOpenGL3_Init(open_gl_version);
	if (opengl_init == false){return false;}

	ImGui::StyleColorsDark();

	return true;
}

// sends a message to engine, asking for ptr to console
void RenderManager::request_debug_console_ptr()
{
	if (message_bus == nullptr)
	{
		PLOGF << "error RenderManager::request_debug_console_ptr()";
		return;
	}

	Message m {
		  engine_channel_id
		, render_channel_id
		, MessageType::REQUEST__CONSOLE_PTR_NON_OWNER
		, nullptr
	};
	
	message_bus->send_direct_message(m);

	return;
}


bool RenderManager::init_shaders()
{
	// this is a default shader to load, regardless of the 
	// project running in the engine
	char const * basic_shader = "basic_shader";

	Message m {
		  resource_channel_id
		, render_channel_id
		, MessageType::REQUEST__RESOURCE
		, (void*)basic_shader
	};
	message_bus->send_direct_message(m);
	active_shader = basic_shader;

	//shaders["textureless"] = std::make_unique<OpenGlShaderProgram>();
	//shaders["textureless"]->compile(textureless_vertex_src, textureless_fragment_src);
	//shaders["textureless"]->use_program();
	
	// heatmap shader
	// greybox shader
	// toon shader

	return true;
}

bool RenderManager::init_geometry()
{
	std::string s = "textured_quad";
	Message m {
		  resource_channel_id
		, render_channel_id
		, MessageType::REQUEST__RESOURCE
		, (void*)s.c_str()
	};
	message_bus->send_direct_message(m);
	
	return true;
}

void RenderManager::handle_message(Message m)
{
	if (m.type == MessageType::SUPPLY__CONSOLE_PTR_NON_OWNER)
	{
		handle_supply_console_ptr_message(m);
	}
	else if (m.type == MessageType::INVOKE__RENDER_COMMAND)
	{
		handle_invoke_render_command(m);
	}
	else if (m.type == MessageType::SUPPLY__RESOURCE)
	{
		handle_supply_resource(m);
	}
}

void RenderManager::handle_supply_console_ptr_message(Message & m)
{
	debug_console = static_cast<Console*>(m.data);
}

void RenderManager::handle_invoke_render_command(Message & m)
{
	auto cs = static_cast<std::string*>(m.data);
	if (contains_term(cs, "render_wireframe="))
	{
		if (contains_term(cs, "true"))
		{
			use_wireframe_rendering = true;
		}
		else if (contains_term(cs, "false"))
		{
			use_wireframe_rendering = false;
		}
		else if (contains_term(cs, "toggle"))
		{
			use_wireframe_rendering = !use_wireframe_rendering;
		}
	}
	else if (contains_term(cs, "triangle2quad"))
	{
		if (contains_term(cs, "toggle"))
		{
			draw_triangle_not_quad = !draw_triangle_not_quad;
		}
	}
	else if (contains_term(cs, "use_shader"))
	{
		if (contains_term(cs, "basic"))
		{
			active_shader = "basic";
		}
		else if (contains_term(cs, "textureless"))
		{
			active_shader = "textureless";
		}
	}
	else if (contains_term(cs, "use_vertex_class_not_quad"))
	{
		if (contains_term(cs, "toggle"))
		{
			use_vertex_class_not_quad = !use_vertex_class_not_quad;
		}
	}
}

void RenderManager::handle_supply_resource(Message & m)
{
	if (m.data == nullptr) 
	{ 
		PLOGE << "ERROR! No resource ptr supplied with SUPPLY__RESOURCE message!";
		message_bus->log_message(m);
		return; 
	}

	auto data_ptr = static_cast<EngineResourceBase*>(m.data);
	if (data_ptr == nullptr) 
	{
		PLOGE << "ERROR! Supplied resource ptr is not castable to EngineResrouceBase!";
		message_bus->log_message(m);
		return; 
	}
	
	if (data_ptr->type == EResourceType::VERTEX_LIST_QUAD)
	{
		auto vert_ptr = static_cast<VertexResource*>(m.data);
		if (vert_ptr == nullptr)
		{
			PLOGE << "ERROR! Supplied resource ptr is of type VERTEX_LIST_QUAD, and is not castable to VertexResource *";
			message_bus->log_message(m);
			return;
		}

		initialize_sprite_quad(vert_ptr, sprite_quad);
	}
	else if (data_ptr->type == EResourceType::SHADER)
	{
		auto shdr = static_cast<ShaderResource*>(m.data);
		if (shdr == nullptr)
		{
			PLOGE << "ERROR! Supplied resource ptr is of type SHADER, and is not castable to ShaderResource *";
			message_bus->log_message(m);
			return;
		}

		// ------------------------------------------------------------

		shader.push_back(std::make_unique<OpenGlShaderProgram>());
		shader.back()->compile(
			  shdr->shader_files["vertex"].c_str()
			, shdr->shader_files["fragment"].c_str()
		);
		active_shader_idx = shader.size() - 1;
		
		// -------------------------------------------------------------
	}
}

void RenderManager::paint_imgui_main_menu_bar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::MenuItem("Load Level"))
			{

			}
			if (ImGui::MenuItem("Load Asset"))
			{

			}
			if (ImGui::MenuItem("Exit Program", "Esc"))
			{
				// request exit here
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
}

void RenderManager::paint_debug_windows()
{
	// we call this every time, and debug console itself is responsible
	// for perfoming an early-out, if it's not visible
	if (debug_console != nullptr)
	{
		debug_console->draw("Debug Console");
	}
	
	// debug window
	if (draw_imgui_debug_window)
	{
		// draw imgui, after drawing the rest of the program for this frame
		ImGui::Begin("Debug Menu");
		ImGui::ColorEdit3("bg color", (float*)&scc);
		ImGui::End();
	}
}

void RenderManager::initialize_sprite_quad(VertexResource const * vrp, OpenGLSpriteQuad & quad)
{
	// this fn only works with the project-defined-datatype, found in
	// the textured_quad.asset

	quad = { 0, 0, 0 };
	glGenVertexArrays(1, &sprite_quad.vao);
	glGenBuffers(1, &sprite_quad.vbo);
	glGenBuffers(1, &sprite_quad.ebo);

	// start work
	glBindVertexArray(sprite_quad.vao);

	// vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, sprite_quad.vbo);
	glBufferData(GL_ARRAY_BUFFER, vrp->vertex_list.size() * sizeof(float), &vrp->vertex_list[0], GL_STATIC_DRAW);

	// index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite_quad.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vrp->index_list.size() * sizeof(float), &vrp->index_list[0], GL_STATIC_DRAW);

	// location, #, datatype, normalized,  stride(beam), offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vrp->beam_size * sizeof(float), (void*)(vrp->vertex_offset * sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vrp->beam_size * sizeof(float), (void*)(vrp->color_offset  * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vrp->beam_size * sizeof(float), (void*)(vrp->texture_offset* sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// I don't know what bind=0 on the buffer would do here
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	billboard_object.set_initialized_vao(
		  sprite_quad.vao
		, vrp->index_list.size()
	);
}

bool RenderManager::update()
{
	// base background color
	glClearColor(
		  (GLclampf)scc.x
		, (GLclampf)scc.y
		, (GLclampf)scc.z
		, (GLclampf)scc.w
	);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader[active_shader_idx]->use_program();
	shader[active_shader_idx]->setInt("_texture", 0);
	billboard_object.draw();

	// imgui; prepare for draw 
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(program_window);
	ImGui::NewFrame();
	// imgui; note: they have to be in this order

	paint_imgui_main_menu_bar();

	paint_debug_windows();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// swapping the buffer is always last step in render update
	SDL_GL_SwapWindow(program_window);
	return true;
}

void RenderManager::shutdown()
{
	PLOGV << witchcraft::log_strings::sdl_begin_shutdown;
	renderer_state = ERendererState::SHUTDOWN_START;
	IMG_Quit();

	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(opengl_context);
	SDL_DestroyWindow(program_window);
	program_window = nullptr;
	
	SDL_Quit();
	renderer_state = ERendererState::SDL_QUIT_OK;
	PLOGV << witchcraft::log_strings::sdl_stop;
	renderer_state = ERendererState::SHUTDOWN_OK;
}


qSceneObject * RenderManager::register_render_object(SDLRenderResource * non_owner, bool is_visible)
{
	// note the cast
	std::unique_ptr<RenderObject> render_object = std::make_unique<qSceneObject>();
	render_object->set_is_visible(is_visible);
	//render_object->set_render_resource(non_owner);

	//

	auto result = static_cast<qSceneObject*>(render_object.get());
	render_objects.push_back(std::move(render_object));
	return result;
}

RenderObject * RenderManager::get_render_object(int id)
{
	//for (auto&& object : render_objects)
	//{
	//	if (object->render_resource->id == id)
	//		return object.get();
	//}

	return nullptr;
}
