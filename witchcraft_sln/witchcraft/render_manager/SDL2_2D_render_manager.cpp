#include "SDL2_2D_render_manager.h"


bool cSDL2RenderManager::init(unsigned int xOffset, unsigned int yOffset, unsigned int Width, unsigned int Height, bool fullScreen, char const * WindowTitle)
{
	std::cout << "\nSDL Initialization start";
	// SDL_Init() returns 0 on success, and a negative number on error
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "\nSDL initialization failure\n" << "Error: " << SDL_GetError();
		// log error: could not initialize SDL Video
		// SDL_QUIT()
		return false;
	}
	
	int flags = 0;

	if (fullScreen)
	{
		flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	flags = flags | SDL_WINDOW_SHOWN;

	std::cout << "\nSDL Window Creation start";

	SDL_CreateWindowAndRenderer(
		  Width
		, Height
		, static_cast<SDL_WindowFlags>(flags)
		, &_sdl_window
		, &_sdl_renderer
	);

	if (_sdl_window == NULL)
	{
		std::cout << "\nSDL Window Creation FAILURE";
		return false;
	}

	SDL_SetWindowTitle(_sdl_window, WindowTitle);

	_sdl_screen_surface = SDL_GetWindowSurface(_sdl_window);

	std::cout << "\nSDL Window Creation Success";
	return true;
}

void cSDL2RenderManager::shutdown()
{
	std::cout << "\nSDL Shutdown begin";
	SDL_DestroyWindow(_sdl_window);
	SDL_Quit();
	std::cout << "\nSDL Shutdown complete";
}

void cSDL2RenderManager::set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect)
{
	r = utility::clamp_value_to_uint8(r);
	g = utility::clamp_value_to_uint8(g);
	b = utility::clamp_value_to_uint8(b);

	SDL_FillRect(_sdl_screen_surface, rect, SDL_MapRGB(_sdl_screen_surface->format, r, g, b));
	SDL_UpdateWindowSurface(_sdl_window);
}
