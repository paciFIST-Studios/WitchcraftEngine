#include "2d_render_manager.h"

bool q2DRenderManager::init(unsigned int xOffset, unsigned int yOffset, unsigned int Width, unsigned int Height, bool fullScreen, char const * WindowTitle)
{
	PLOGV << witchcraft::log_strings::sdl_start;
	// SDL_Init() returns 0 on success, and a negative number on error
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		PLOGF << witchcraft::log_strings::sdl_init_failure << "\nError: " << SDL_GetError();
		return false;
	}
	
	// Flags are set with bitmasking
	// NOTE: DO NOT use the flag SDL_WINDOW_OPENGL
	// sdl chooses the renderer, use the 
	int flags = 0;

	if (fullScreen)
	{
		flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	flags = flags | SDL_WINDOW_SHOWN;

	PLOGD << witchcraft::log_strings::sdl_window_init;
	SDL_CreateWindowAndRenderer(
		  Width
		, Height
		, static_cast<SDL_WindowFlags>(flags)
		, &_window
		, &_current_renderer
	);

	if (_window == NULL)
	{
		PLOGF << witchcraft::log_strings::sdl_window_init_failure << "\nError: " << SDL_GetError();
		return false;
	}

	SDL_SetWindowTitle(_window, WindowTitle);

	_rendering_surface = SDL_GetWindowSurface(_window);
	PLOGV << witchcraft::log_strings::sdl_window_init_success;

	return true;
}

bool q2DRenderManager::update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// check messages
		switch (event.type)
		{
		case SDL_QUIT: { return false; }

		case SDL_KEYDOWN:
		{
			// [ESC]
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				PLOGI << witchcraft::log_strings::sdl_break_event_polling;
				return false;
			}
			// others
		}

		} //end switch
	} //end message

	// clear screen
	SDL_RenderClear(_current_renderer);
	render_all_objects();
	SDL_RenderPresent(_current_renderer);

	return true;
}

void q2DRenderManager::shutdown()
{
	PLOGV << witchcraft::log_strings::sdl_begin_shutdown;
	SDL_DestroyWindow(_window);
	SDL_FreeSurface(_rendering_surface);
	SDL_DestroyRenderer(_current_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
	PLOGV << witchcraft::log_strings::sdl_stop;
}

void q2DRenderManager::render_all_objects()
{
	if (_render_objects.size() < 1)
		return;

	for (auto&& object : _render_objects)
	{
		if (object->is_visible() == false)
			continue;

		object->update();
		SDL_Rect position;

		auto position_tuple = object->get_position();
		position.x = int(std::get<0>(position_tuple));
		position.y = int(std::get<1>(position_tuple));

		SDL_RenderCopy(
			  _current_renderer
			, object->_render_resource->_texture
			, &object->_render_rect
			, &position
		);
	}
}

void q2DRenderManager::set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect)
{
	r = utility::clamp_value_to_uint8(r);
	g = utility::clamp_value_to_uint8(g);
	b = utility::clamp_value_to_uint8(b);

	SDL_FillRect(_rendering_surface, rect, SDL_MapRGB(_rendering_surface->format, r, g, b));
	SDL_UpdateWindowSurface(_window);
}
