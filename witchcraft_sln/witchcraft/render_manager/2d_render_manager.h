#ifndef SDL2_2D_RENDER_MANAGER_H
#define SDL2_2D_RENDER_MANAGER_H

// stdlib
#include <iomanip>
#include <iostream>
#include <list>
#include <math.h>
#include <memory>
#include <string>
#include <sstream>
#include <time.h>

// sdl
#include <SDL.h>
#include <SDL_surface.h>
#include <SDL_video.h>

// rapidxml
#include "../../lib/rapidxml/rapidxml.hpp"
#include "../../lib/rapidxml/rapidxml_utils.hpp"
#define XML rapidxml

// logging
#include <plog/Log.h>

// witchcraft
#include "../utility/utility.h"
#include "../string_constants.h"

#include "../resource_manager/resource.h"
#include "render_resource.h"
#include "SDL_2D_render_object.h"
#include "render_manager_base.h"

#define RENDER_OBJECT_VECTOR__TYPE std::vector<std::unique_ptr<cSDL2DRenderObject>>

// Depends on:
//	c2DSpriteObject
//	cRenderManagerBase
//	cSDL2DRenderObject
//	cRenderResource
//
//	This class serves to
//	1. create a window
//	2. initialize hardware
//	3. create / load / unload graphics resource
//	4. create / load / unload render graphics
class cSDL2RenderManager : public cRenderManagerBase
{
private:
protected:

	static std::unique_ptr<cSDL2RenderManager> _SDL2D_render_manager;

	RENDER_OBJECT_VECTOR__TYPE _render_objects;

public:
	cSDL2RenderManager() {}

	static cSDL2RenderManager * get_SDL2D_render_manager();
	
	// the program window
	SDL_Window * _window;

	// the render SDL uses
	SDL_Renderer * _current_renderer;

	// Surface is software, texture is hardware (GPU rendering)
	SDL_Surface * _rendering_surface;

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
	std::unique_ptr<cResource> load_resource_from_xml(XML::xml_node<> const & xml);
	void render_all_objects();

};

#endif // SDL2_2D_RENDER_MANAGER_H
