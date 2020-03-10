#include "SDL2_2D_render_manager.h"


bool cSDL2RenderManager::init(unsigned int xOffset, unsigned int yOffset, unsigned int Width, unsigned int Height, bool fullScreen, char * WindowTitle)
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

	if (Width == 0 || Height == 0)
	{
		flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

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

void cSDL2RenderManager::show_video_info()
{
}
