#include "render_manager_2d.h"

bool q2DRenderManager::init(unsigned int xOffset, unsigned int yOffset, unsigned int Width, unsigned int Height, bool fullScreen, char const * WindowTitle)
{
	PLOGV << witchcraft::log_strings::sdl_start;

	int flags = 0;

	// use for setting flag options
	if (true)
	{
		flags = flags | SDL_INIT_EVERYTHING;
	}

	// SDL_Init() returns 0 on success, and a negative number on error
	if (SDL_Init(flags) < 0)
	{
		PLOGF << witchcraft::log_strings::sdl_init_failure << "\nError: " << SDL_GetError();
		return false;
	}


	flags = 0;
	// use for setting SDL_Image flag options
	if(true)
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


	flags = 0;
	if (fullScreen)
	{
		flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	flags = flags | SDL_WINDOW_SHOWN;

	// NOTE: DO NOT use the flag SDL_WINDOW_OPENGL
	// sdl chooses the renderer, use the 
	PLOGD << witchcraft::log_strings::sdl_window_init;
	SDL_CreateWindowAndRenderer(
		  Width
		, Height
		, static_cast<SDL_WindowFlags>(flags)
		, &program_window
		, &active_renderer
	);

	if (program_window == NULL)
	{
		PLOGF << witchcraft::log_strings::sdl_window_init_failure << "\nError: " << SDL_GetError();
		return false;
	}

	SDL_SetWindowTitle(program_window, WindowTitle);

	rendering_surface = SDL_GetWindowSurface(program_window);
	PLOGV << witchcraft::log_strings::sdl_window_init_success;

	return true;
}

bool q2DRenderManager::update()
{
	SDL_RenderClear(active_renderer);
	render_all_objects();
	SDL_RenderPresent(active_renderer);
	return true;
}

void q2DRenderManager::shutdown()
{
	PLOGV << witchcraft::log_strings::sdl_begin_shutdown;
	IMG_Quit();
	SDL_DestroyWindow(program_window);
	SDL_FreeSurface(rendering_surface);
	SDL_DestroyRenderer(active_renderer);
	SDL_Quit();
	PLOGV << witchcraft::log_strings::sdl_stop;
}

void q2DRenderManager::render_all_objects()
{
	if (render_objects.size() < 1)
		return;

	for (auto&& object : render_objects)
	{
		if (object->is_visible() == false)
			continue;

		object->update();
		SDL_Rect position;

		auto position_tuple = object->get_position();
		position.x = int(std::get<0>(position_tuple));
		position.y = int(std::get<1>(position_tuple));
		position.w = object->render_rect.w;
		position.h = object->render_rect.h;

		SDL_RenderCopy(
			  active_renderer
			, object->render_resource->texture
			, &object->render_rect
			, &position
		);
	}
}

void q2DRenderManager::set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect)
{
	r = utility::clamp_value_to_uint8(r);
	g = utility::clamp_value_to_uint8(g);
	b = utility::clamp_value_to_uint8(b);

	SDL_FillRect(rendering_surface, rect, SDL_MapRGB(rendering_surface->format, r, g, b));
	SDL_UpdateWindowSurface(program_window);
}

void q2DRenderManager::register_render_object(qRenderResource * non_owner, bool is_visible)
{
	auto render_object = std::make_unique<RenderObject2D>();
	render_object->set_is_visible(is_visible);
	render_object->set_render_resource(non_owner);

	// This SDL_Rect covers the parts of the texture we will display
	// We're sizing it to include the entire texture
	auto wh = non_owner->get_width_height();
	render_object->render_rect.w = std::get<0>(wh);
	render_object->render_rect.h = std::get<1>(wh);
	render_object->render_rect.x = 0;
	render_object->render_rect.y = 0;

	render_objects.push_back(std::move(render_object));
}

RenderObject2D * q2DRenderManager::get_render_object(int id)
{
	for (auto&& object : render_objects)
	{
		if (object->render_resource->get_resource_id() == id)
			return object.get();
	}

	return nullptr;
}
