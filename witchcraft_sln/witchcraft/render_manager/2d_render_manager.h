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
#include <SDL_image.h>
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

#define RENDER_OBJECTS_VECTOR std::vector<std::unique_ptr<RenderObject2D>>

// Depends on:
//	c2DSpriteObject
//	qRenderManagerBase
//	RenderObject2D
//	qRenderResource
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

	static std::unique_ptr<q2DRenderManager> SDL2_2D_render_manager;

	RENDER_OBJECTS_VECTOR render_objects;

public:
	q2DRenderManager() {}
	
	SDL_Window * program_window;

	SDL_Renderer * active_renderer;

	SDL_Surface * rendering_surface;

	bool init(
		  unsigned int xOffset = SDL_WINDOWPOS_UNDEFINED
		, unsigned int yOffset = SDL_WINDOWPOS_UNDEFINED
		, unsigned int Width   = 0
		, unsigned int Height  = 0
		, bool fullScreen	   = false
		, char const * WindowTitle = 0
	);

	bool update();

	void shutdown();

	void render_all_objects();

	void set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect);
	
	bool register_render_object(qRenderResource * non_owner, bool is_visible = true)
	{
		auto render_object = std::make_unique<RenderObject2D>();
		render_object->set_is_visible(is_visible);
		render_object->set_resource(non_owner);
		render_objects.push_back(std::move(render_object));
		return true;
	}

	void set_render_object_at_position(RenderObject2D const & obj, unsigned int x, unsigned int y);

};

#endif // TWO_D_RENDER_MANAGER_H
