#include "render_manager.h"

bool RenderManager::init(unsigned int xOffset, unsigned int yOffset, unsigned int Width, unsigned int Height, bool fullScreen, char const * WindowTitle)
{
	PLOGV << witchcraft::log_strings::sdl_start;

	// -1 == error, 0 == success
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		PLOGF << witchcraft::log_strings::sdl_init_failure << "\nError: " << SDL_GetError();
		return false;
	}

	PLOGD << "sdl-opengl window init";
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
		PLOGF << "could not init sdl opengl";
		return false;
	}
	else
	{
		PLOGV << "sdl window created";
	}


	// OpenGL 3.2 rendering context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	opengl_context = SDL_GL_CreateContext(program_window);
	if (opengl_context == nullptr)
	{
		PLOGF << "could not init opengl context";
		SDL_DestroyWindow(program_window);
		program_window = nullptr;
		SDL_Quit();
		return false;
	}
	else
	{
		PLOGV << "opengl context created";
	}

	// set glew for opengl 3+
	glewExperimental = GL_TRUE;
	auto error = glewInit();
	if (error != GLEW_OK)
	{
		PLOGF << "Failed to init GLEW!";
		SDL_GL_DeleteContext(opengl_context);
		SDL_DestroyWindow(program_window);
		program_window = nullptr;
		SDL_Quit();
		return false;
	}
	else
	{
		auto render_str = glGetString(GL_RENDERER);
		auto ver_str = glGetString(GL_VERSION);

		PLOGV << "glew helper lib for opengl, initialized";
		{
			bool log_personal_data = true;
			if (log_personal_data)
			{
				PLOGV << "hardware info: " << render_str;
			}
		}
		PLOGV << "opengl version: " << ver_str;
	}



	SDL_GL_SetSwapInterval(1); // use VSYNC
	glEnable(GL_DEPTH_TEST);	// only draw closest pixel to screen
	glDepthFunc(GL_LESS);	// for depth test, smaller == closer


	int flags = 0;
	// use for setting SDL_Image flag options
	if (true)
	{
		flags = flags | IMG_INIT_JPG;
		flags = flags | IMG_INIT_PNG;
	}
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

	//init_shaders();
	//init_geometry();
	//init_textures();

	PLOGV << witchcraft::log_strings::sdl_window_init_success;

	return true;
}

bool RenderManager::init_shaders()
{
	GLint status;
	char err_buff[512];

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// compile vertex shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		glGetShaderInfoLog(vertex_shader, sizeof(err_buff), NULL, err_buff);
		err_buff[sizeof(err_buff)-1] = '\0';
		PLOGF << "FAILURE: Vertex Shader Compile: " << err_buff << std::endl;
		return false;
	}

	// compile fragment shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		glGetShaderInfoLog(fragment_shader, sizeof(err_buff), NULL, err_buff);
		err_buff[sizeof(err_buff) - 1] = '\0';
		PLOGF << "FAILURE: Fragment Shader Compile: " << err_buff << std::endl;
		return false;
	}
	
	// link vertex and fragment shaders
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glBindFragDataLocation(shader_program, 0, "out_Color");
	glLinkProgram(shader_program);
	glUseProgram(shader_program);

	return true;
}

bool RenderManager::init_geometry()
{
	// populate vertex buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	// populate element buffer
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	// bind vertex position attribute
	GLint pos_attrib_loc = glGetAttribLocation(shader_program, "in_Position");
	glVertexAttribPointer(pos_attrib_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(pos_attrib_loc);

	// bind vertex texture coordinate attribute
	GLint tex_attrib_loc = glGetAttribLocation(shader_program, "in_Texcoord");
	glVertexAttribPointer(tex_attrib_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tex_attrib_loc);

	return true;
}

bool RenderManager::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	SDL_GL_SwapWindow(program_window);



	//SDL_RenderClear(active_renderer);
	//render_visible_scene_back_to_front();
	//SDL_RenderPresent(active_renderer);

	//glViewport(0, 0, 100, 100);
	//glClearColor(1.f, 1.f, 1.f, 1.f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//SDL_GL_SwapWindow(program_window);

	return true;
}

void RenderManager::shutdown()
{
	PLOGV << witchcraft::log_strings::sdl_begin_shutdown;
	IMG_Quit();
	

	SDL_GL_DeleteContext(opengl_context);
	SDL_DestroyWindow(program_window);
	program_window = nullptr;

	//SDL_FreeSurface(rendering_surface);
	//SDL_DestroyRenderer(active_renderer);
	SDL_Quit();
	PLOGV << witchcraft::log_strings::sdl_stop;
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
