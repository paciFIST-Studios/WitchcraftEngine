#ifndef TWO_D_RENDER_MANAGER_H
#define TWO_D_RENDER_MANAGER_H

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

// witchcraft
#include "../engine/engine_object.h"
#include "../resource_manager/resource.h"
#include "../string_constants.h"
#include "../utility/utility.h"
#include "2d_render_object.h"
#include "render_resource.h"

#define RENDER_OBJECTS_VECTOR std::vector<std::unique_ptr<c2DRenderObject>>

// Depends on:
//	c2DSpriteObject
//	qRenderManagerBase
//	c2DRenderObject
//	cRenderResource
//
//	This class serves to
//	1. create a window
//	2. initialize hardware
//	3. create / load / unload graphics resource
//	4. create / load / unload render graphics
class q2DRenderManager : public qEngineObject
{
private:
protected:

	static std::unique_ptr<q2DRenderManager> _SDL2D_render_manager;

	RENDER_OBJECTS_VECTOR _render_objects;

public:
	q2DRenderManager() {}

	static q2DRenderManager * get_SDL2D_render_manager();
	
	// the program window
	SDL_Window * _window;

	// the render SDL uses
	SDL_Renderer * _current_renderer;

	// Surface is software, texture is hardware (GPU rendering)
	SDL_Surface * _rendering_surface;

	bool init(
		  unsigned int xOffset = SDL_WINDOWPOS_UNDEFINED
		, unsigned int yOffset = SDL_WINDOWPOS_UNDEFINED
		, unsigned int Width   = 0
		, unsigned int Height  = 0
		, bool fullScreen	   = false
		, char const * WindowTitle = 0
	);

	void shutdown();

	void set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect);

	bool update();
	void render_all_objects();

	//std::unique_ptr<qResource> load_resource_from_xml(XML::xml_node<> const & xml);
};

#endif // TWO_D_RENDER_MANAGER_H
