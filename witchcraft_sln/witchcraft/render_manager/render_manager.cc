#include "render_manager.h"

bool RenderManager::init_system(unsigned xOffset, unsigned yOffset, unsigned Width, unsigned Height, bool fullScreen, char const * WindowTitle)
{
	PLOGI << witchcraft::log_strings::render_manager_system_init_start;

	if (false == init_sdl(xOffset, yOffset, Width, Height, WindowTitle))
	{
		// couldn't init sdl
		return false;
	}

	if (false == init_sdl_image())
	{
		// couldn't init sdl_image (the lib for PNG files, etc)
		return false;
	}

	if (false == init_opengl())
	{
		// couldn't init opengl
		return false;
	}

	// NOTE: this initialization has to come AFTER the opengl init
	if (false == init_imgui())
	{
		return false;
	}

	// not sure where to set this, actually
	scc = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);

	SDL_GL_SetSwapInterval(1);	// use VSYNC
	glEnable(GL_DEPTH_TEST);	// only draw closest pixel to screen
	glDepthFunc(GL_LESS);		// for depth test, smaller == closer
	
	if (false == init_shaders()) 
	{ 
		return false; 
	}
	if (false == init_geometry()) 
	{ 
		return false; 
	}

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

	renderer_state = ERendererState::OPENGL_INIT_OK;

	return true;
}

bool RenderManager::init_sdl_image()
{
	int flags = 0;
	flags = flags | IMG_INIT_JPG;
	flags = flags | IMG_INIT_PNG;

	// not sure what this returns, actually
	int icode = IMG_Init(flags);
	if ((icode & flags) != flags)
	{
		// failed to init
		PLOGF << witchcraft::log_strings::sdl_init_failure
			<< "\nError: Could not initialize PNG + JPG support!\n"
			<< IMG_GetError();
		return false;
	}

	return true;
}

bool RenderManager::init_imgui()
{
	// NOTE: these early-out returns are basically
	// only present for style, several of these fns never
	// return false

	IMGUI_CHECKVERSION();
	
	auto imgui_context = ImGui::CreateContext();
	if (imgui_context == nullptr)
	{
		return false;
	}

	ImGuiIO &io = ImGui::GetIO();
	
	auto sdl_init = ImGui_ImplSDL2_InitForOpenGL(program_window, opengl_context);
	if (sdl_init == false)
	{
		return false;
	}

	auto opengl_init = ImGui_ImplOpenGL3_Init(open_gl_version);
	if (opengl_init == false)
	{
		return false;
	}

	ImGui::StyleColorsDark();

	return true;
}

bool RenderManager::init_shaders()
{

	// Vertex Shader
	if (false== compile_shader(vertex_shader_id, GL_VERTEX_SHADER, vertex_shader_src))
	{
		return false;
	}

	// Fragment Shader
	if (false == compile_shader(fragment_shader_id, GL_FRAGMENT_SHADER, fragment_shader_src))
	{
		return false;
	}

	// shader program
	if (false == link_shader_program(vertex_shader_id, fragment_shader_id, shader_program_id))
	{
		return false;
	}

	// from here, we'll just rely on the shader program
	glDetachShader(shader_program_id, vertex_shader_id);
	glDetachShader(shader_program_id, fragment_shader_id);
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
	
	return true;
}

bool RenderManager::init_geometry()
{
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), &verticies[0], GL_STATIC_DRAW);
	return true;
}

bool RenderManager::compile_shader(GLuint id, GLenum type, char const * src)
{
	id = glCreateShader(type);
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	GLint status;
	GLint len;
	std::string msg;

	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		msg = std::string(len+1, '\0');
		glGetShaderInfoLog(id, len, &len, &msg[0]);
	}

	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if(status == FALSE)
	{
		PLOGF << "FAIL: shader compile: " << Glenum_to_str[type];
		if (len > 0) { PLOGF << "FAIL: shader compile: " << msg; }
		return false;
	}

	PLOGV << "shader compile: success: " << Glenum_to_str[type];
	if (len > 0) { PLOGV << msg; }
	return true;
}

bool RenderManager::link_shader_program(GLuint vert_id, GLuint frag_id, GLuint prog_id)
{
	prog_id = glCreateProgram();
	glAttachShader(prog_id, vert_id);
	glAttachShader(prog_id, frag_id);

	glLinkProgram(prog_id);

	GLint status;
	GLint len;
	std::string msg;

	glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		msg = std::string(len+1, '\0');
		glGetProgramInfoLog(prog_id, len, &len, &msg[0]);
	}

	glGetProgramiv(prog_id, GL_COMPILE_STATUS, &status);
	if (status == FALSE)
	{
		PLOGF << "FAIL: shader-program compile";
		if (len > 0) { PLOGF << "FAIL: shader-program compile: " << msg; }
		return false;
	}
	else
	{
		PLOGV << "shader-program compile: success";
		if (len > 0) { PLOGV << msg; }
	}

	return true;
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

void RenderManager::paint_imgui_console_window()
{
	static int items_added = -1;

	static std::vector<std::string> history;

	ImGui::Begin("Debug Console");
	ImGui::TextWrapped(
		"This is an implementation of a debug console"
	);
	ImGui::TextWrapped("Type \"help\" for help, press [TAB] to use text completion");

	if (ImGui::SmallButton("Add Line"))
	{
		std::stringstream ss;
		ss << "line " << items_added++;
		history.push_back(ss.str());
	}

	ImGui::Separator();

	ImGui::BeginChild("Scroll Region", ImVec2(0, -30), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	for (auto item : history)
	{
		ImVec4 color;
		if (item == "ERROR") 
		{
			color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
		}
		else
		{
			color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
		}

		ImGui::Text(item.c_str());
	}

	ImGui::EndChild();
	ImGui::Separator();

	static char buf[1024];
	ImGui::InputText("Input", buf, IM_ARRAYSIZE(buf));
	
	ImGui::SetItemDefaultFocus();

	ImGui::End();

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
	glUseProgram(shader_program_id);

	// imgui prepare for draw 
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(program_window);
	ImGui::NewFrame();
	// imgui; note: they have to be in this order

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_array_id);
	glVertexAttribPointer(
		0			// ?
		, 3			// size
		, GL_FLOAT	// type
		, GL_FALSE	// normalized
		, 0			// stride
		, (void*)0	// buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);

	paint_imgui_main_menu_bar();

	if (draw_imgui_debug_console)
	{
		paint_imgui_console_window();
	}


	// debug window
	if (draw_imgui_debug_window)
	{
		// draw imgui, after drawing the rest of the program for this frame
		ImGui::Begin("Debug Menu");
		ImGui::ColorEdit3("bg color", (float*)&scc);
		ImGui::End();
	}


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


	//SDL_FreeSurface(rendering_surface);
	//SDL_DestroyRenderer(active_renderer);
	SDL_Quit();
	renderer_state = ERendererState::SDL_QUIT_OK;
	PLOGV << witchcraft::log_strings::sdl_stop;
	renderer_state = ERendererState::SHUTDOWN_OK;
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

			SDL_RenderCopy(
				  active_renderer
				, obj->render_resource->texture
				, &src_rect
				, &dest_rect
			);
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

qSceneObject * RenderManager::register_render_object(qRenderResource * non_owner, bool is_visible)
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
		if (object->render_resource->get_resource_id() == id)
			return object.get();
	}

	return nullptr;
}
