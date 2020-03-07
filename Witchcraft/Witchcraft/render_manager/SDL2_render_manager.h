#ifndef SDL2_RENDER_MANAGER_H
#define SDL2_RENDER_MANAGER_H

#include <SDL.h>
#include <SDL_Image.h>
#include <string>
#include <sstream>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <math.h>

#include "../../lib/rapidxml/rapidxml.hpp"

#include "../resource_manager/resource.h"

#include "SDL_render_object.h"
#include "2D_render_manager.h"


// Depends on:
//	cSpriteObject
//	c2DRenderManager
//	cSDLRenderObject
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
	cSDL2RenderManager();
	static cSDL2RenderManager * _SDL2D_render_manager;
public:
	static cSDL2RenderManager * get_SDL2D_render_manager();
	SDL_surface * _render_window;
	std::stringstream _video_info;

	bool init(
		  unsigned int Width = 800
		, unsigned int Height = 600
		, bool fullScreen = false
		, char * WindowTitle = 0
	);

	void free();
	bool update();
	void toggle_full_screen();
	cResource * loadResourceFromXML(std::string xml);
	void render_all_objects();
	std::list<cSDLRenderObject*> _render_objects;
};

#endif // SDL2_RENDER_MANAGER_H
