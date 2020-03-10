#ifndef SDL2_2D_RENDER_MANAGER_H
#define SDL2_2D_RENDER_MANAGER_H

// stdlib
#include <iomanip>
#include <iostream>
#include <list>
#include <math.h>
#include <string>
#include <sstream>
#include <time.h>

// sdl
#define SDL_MAIN_HANDLED // NOTE: we MUST define this, in order to use a non-SDL main function (ie: insertion point)
#include <SDL.h>
#include <SDL_surface.h>
#include <SDL_video.h>

// rapidxml
#include "../../lib/rapidxml/rapidxml.hpp"

// witchcraft
#include "../utility/utility.h"
#include "../resource_manager/resource.h"
#include "SDL_2D_render_object.h"
#include "2D_render_manager.h"


// Depends on:
//	c2DSpriteObject
//	c2DRenderManager
//	cSDL2DRenderObject
//	cRenderResource
//
//	This class serves to
//	1. create a window
//	2. initialize hardware
//	3. create / load / unload graphics resource
//	4. create / load / unload render graphics
class cSDL2RenderManager : public c2DRenderManager
{
private:
protected:

	static cSDL2RenderManager * _SDL2D_render_manager;
public:
	cSDL2RenderManager() {}

	static cSDL2RenderManager * get_SDL2D_render_manager();
	SDL_Window * _sdl_window;
	SDL_Renderer * _sdl_renderer;

	SDL_Surface * _sdl_screen_surface;
	

	std::stringstream _video_info;

	bool init(
		  unsigned int xOffset = SDL_WINDOWPOS_UNDEFINED
		, unsigned int yOffset = SDL_WINDOWPOS_UNDEFINED
		, unsigned int Width = 0
		, unsigned int Height = 0
		, bool fullScreen = false
		, char const * WindowTitle = 0
	);

	void shutdown();

	void set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect);

	void free();
	bool update();
	void toggle_full_screen();
	cResource * loadResourceFromXML(std::string xml);
	void render_all_objects();
	std::list<cSDL2DRenderObject*> _render_objects;
};



#endif // SDL2_2D_RENDER_MANAGER_H
