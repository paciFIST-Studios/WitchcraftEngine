#include "render_manager.h"

bool RenderManager::init_system(unsigned xOffset, unsigned yOffset, unsigned Width, unsigned Height, bool fullScreen, char const * WindowTitle)
{
	PLOGI << witchcraft::log_strings::render_manager_system_init_start;

	if ( ! init_sdl(xOffset, yOffset, Width, Height, WindowTitle))
	{return false;}
	//if ( ! init_sdl_image()){ return false; }
	
	if ( ! init_opengl())	{ return false; }

	// NOTE: this initialization has to come AFTER the opengl init
	if ( ! init_imgui())	{ return false; }

	// init shaders
	if ( ! init_shaders())	{ return false; }
	if ( ! init_geometry())	{ return false; }

	// sends a message to engine, asking for ptr to console
	init_get_debug_console();

	PLOGV << witchcraft::log_strings::render_manager_system_init_end;
	
	renderer_state = ERendererState::UPDATE_OK;
	return true;
}

bool RenderManager::init_sdl(unsigned xOffset, unsigned yOffset, unsigned Width, unsigned Height, char const * WindowTitle)
{
	PLOGI << witchcraft::log_strings::sdl_start;

	// -1 == error, 0 == success
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		PLOGF << witchcraft::log_strings::sdl_init_failure << "\nError: " << SDL_GetError();
		return false;
	}
	else
	{
		PLOGV << witchcraft::log_strings::sdl_ok;
	}

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
	else
	{
		PLOGV << witchcraft::log_strings::sdl_window_ok;
	}

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
		PLOGV << "gfx card info: " << glGetString(GL_RENDERER);
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

void RenderManager::init_get_debug_console()
{
	if (message_bus == nullptr)
	{
		PLOGF << "error RenderManager::init_get_debug_console()";
		return;
	}

	Message m{
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
	shaders["basic"] = std::make_unique<OpenGlShaderProgram>();
	shaders["basic"]->compile(basic_vertex_src, basic_fragment_src);

	// heatmap shader
	// greybox shader
	// toon shader

	active_shader_program_id = shaders["basic"]->get_shader_program_id();
	return true;
}

bool RenderManager::init_geometry()
{
	std::string s = "textured_quad";
	Message m
	{
		  resource_channel_id
		, render_channel_id
		, MessageType::REQUEST__RESOURCE
		, (void*)s.c_str()
	};
	message_bus->send_direct_message(m);

	
	// ------------------------------------------------------------------
	// matricies
	//
	//w = 0;
	//h = 0;
	//SDL_GetWindowSize(program_window, &w, &h);
	//
	//
	//orthographic_projection_matrix = glm::ortho(
	//	  0.0f			//	left
	//	, (float)w		//	right
	//	, (float)h		//	bottom
	//	, 0.0f			//	top
	//	,-1.0f			//	near
	//	, 1.0f			//	far
	//);
	//
	//perspective_projection_matrix = glm::perspective(
	//	  glm::radians(45.0f)	// fov
	//	, (float)w / (float)h	// aspect ratio
	//	, 0.1f					// near
	//	, 100.f					// far
	//);
	//
	//// start with identity matrix
	//model_matrix = glm::mat4(1.0f);
	//// rotate by -55 degrees around x+ axis
	//model_matrix = glm::rotate(model_matrix, glm::radians(-0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//
	//// view matrix moves camera back
	//view_matrix = glm::mat4(1.0f);
	//view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -3.0f));
	//
	//
	//shader_id = active_shader_program_id;
	//int model_loc = glGetUniformLocation(shader_id, "model");
	//int view_loc = glGetUniformLocation(shader_id, "view");
	//int proj_loc = glGetUniformLocation(shader_id, "projection");
	//
	//glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
	//glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view_matrix[0][0]);
	//glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(orthographic_projection_matrix));
	
	return true;
}

void RenderManager::handle_message(Message m)
{
	if (m.type == MessageType::SUPPLY__CONSOLE_PTR_NON_OWNER)
	{
		debug_console = static_cast<Console*>(m.data);
		return;
	}
	else if (m.type == MessageType::INVOKE__RENDER_COMMAND)
	{
		auto command_str = static_cast<std::string*>(m.data);
		if (contains_term(command_str, "render_wireframe="))
		{
			if (contains_term(command_str, "true"))
			{
				use_wireframe_rendering = true;
			}
			else if (contains_term(command_str, "false"))
			{
				use_wireframe_rendering = false;
			}
			else if (contains_term(command_str, "toggle"))
			{
				use_wireframe_rendering = !use_wireframe_rendering;
			}
		}
		else if (contains_term(command_str, "triangle2quad"))
		{
			if (contains_term(command_str, "toggle"))
			{
				draw_triangle_not_quad = !draw_triangle_not_quad;
			}
		}
	}
	else if (m.type == MessageType::SUPPLY__RESOURCE)
	{
		if (m.data == nullptr) { return; }
		auto erbp = static_cast<EngineResourceBase*>(m.data);
		
		if (erbp != nullptr) 
		{
			if (erbp->type == EResourceType::VERTEX_LIST)
			{
				auto vrp = static_cast<VertexResource*>(m.data);
				if (vrp != nullptr)
				{
					GLuint vao, vbo, ebo;
					glGenVertexArrays(1, &vao);
					glGenBuffers(1, &vbo);
					glGenBuffers(1, &ebo);

					sprite_quad.vao = vao;
					sprite_quad.vbo = vbo;
					sprite_quad.ebo = ebo;

					// start work
					glBindVertexArray(vao);

					// vertex buffer
					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBufferData(
						GL_ARRAY_BUFFER
						, sizeof(vrp->vertex_list.size())
						, &vrp->vertex_list[0]
						, GL_STATIC_DRAW
					);

					// index buffer
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
					glBufferData(
						GL_ELEMENT_ARRAY_BUFFER
						, sizeof(vrp->index_list)
						, &vrp->index_list[0]
						, GL_STATIC_DRAW
					);

					// basic shader uses vec3(location=0)
					// , and v2(location=1) inputs
					glVertexAttribPointer(
						  0		// location 0
						, 2
						, GL_FLOAT
						, GL_FALSE
						, vrp->vertex_stride
						, (void*)vrp->vertex_offset
					);
					glEnableVertexAttribArray(0);

					glVertexAttribPointer(
						  1
						, 2
						, GL_FLOAT
						, GL_FALSE
						, vrp->texture_stride
						, (void*)vrp->texture_offset
					);
					glEnableVertexAttribArray(1);

					shaders["basic"]->use_program();
					shaders["basic"]->setInt("_texture", 0);
				}
			}
		}
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite_texture.get_texture_id());
	shaders["basic"]->use_program();
	


	glBindVertexArray(sprite_quad.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// imgui; prepare for draw 
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(program_window);
	ImGui::NewFrame();
	// imgui; note: they have to be in this order


	if (use_wireframe_rendering)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

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


void RenderManager::render_visible_sprites_back_to_front()
{
	if (scene_manager == nullptr)
	{
		PLOGE << "ERROR:  No scene manager detected!";
		return;
	}

	auto layers = scene_manager->get_layers_ptrs_vector();
	for (auto&& layer : layers)
	{
		if (layer == nullptr)
			continue;

		if (layer->get_is_visible() == false)
		{
			continue;
		}

		auto objects = layer->get_layer_objects();

		for (auto&& obj : objects)
		{
			if (obj == nullptr)
				continue;

			if (obj->is_visible() == false)
				continue;

			//draw_sprite(*obj);
		}
	}
}

void RenderManager::render_visible_scene_back_to_front()
{
	if (scene_manager == nullptr)
		return; // error?

	auto layers = scene_manager->get_layers_ptrs_vector();
	for (auto&& layer : layers)
	{
		if (layer == nullptr)
			continue;

		if (layer->get_is_visible() == false)
			continue;

		auto objects = layer->get_layer_objects();

		for (auto&& obj : objects)
		{
			if (obj == nullptr)
				continue;

			if (obj->is_visible() == false)
				continue;

			// NOTE: we should move this out of the rendering area
			//// is this where the tick for the object is called? Is that okay?
			//obj->update();

			SDL_Rect dest_rect;
			SDL_Rect src_rect = obj->render_resource->get_renderable_rect();
			
			auto layer_pos = layer->get_offset();
			auto obj_pos = obj->get_position();
		
			dest_rect.x = int(std::get<0>(layer_pos) + std::get<0>(obj_pos));
			dest_rect.y = int(std::get<1>(layer_pos) + std::get<1>(obj_pos));
			
			auto scale = obj->get_scale();
			dest_rect.w = int(src_rect.w * std::get<0>(scale));
			dest_rect.h = int(src_rect.h * std::get<1>(scale));

			//SDL_RenderCopy(
			//	  active_renderer
			//	, obj->render_resource->texture
			//	, &src_rect
			//	, &dest_rect
			//);
		}
	}
}

void RenderManager::set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect)
{
	r = utility::clamp_to_0_255(r);
	g = utility::clamp_to_0_255(g);
	b = utility::clamp_to_0_255(b);

	SDL_FillRect(rendering_surface, rect, SDL_MapRGB(rendering_surface->format, r, g, b));
	SDL_UpdateWindowSurface(program_window);
}

qSceneObject * RenderManager::register_render_object(SDLRenderResource * non_owner, bool is_visible)
{
	// note the cast
	std::unique_ptr<RenderObject2D> render_object = std::make_unique<qSceneObject>();
	render_object->set_is_visible(is_visible);
	render_object->set_render_resource(non_owner);

	//

	auto result = static_cast<qSceneObject*>(render_object.get());
	render_objects.push_back(std::move(render_object));
	return result;
}

RenderObject2D * RenderManager::get_render_object(int id)
{
	for (auto&& object : render_objects)
	{
		if (object->render_resource->id == id)
			return object.get();
	}

	return nullptr;
}
