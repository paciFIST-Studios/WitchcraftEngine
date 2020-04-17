#ifndef RENDER_MANAGER_TWO_D_H
#define RENDER_MANAGER_TWO_D_H

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
#include "render_object_2d.h"
#include "render_resource.h"

#include "../scene_manager/scene_manager_2d.h"
#include "../scene_manager/scene_object.h"

class SceneManager2D;
class qSceneObject;

class RenderManager2D : public qEngineObject
{
public:
	typedef std::vector<std::unique_ptr<RenderObject2D>> RenderObjectsVector;
	
private:
protected:
	static std::unique_ptr<RenderManager2D> SDL2_2D_render_manager;

	RenderObjectsVector render_objects;

	SceneManager2D * scene_manager = nullptr;
	
	unsigned int screen_width = 0;
	unsigned int screen_height = 0;

public:
	RenderManager2D() {}
	
	SDL_Window * program_window = nullptr;

	SDL_Renderer * active_renderer = nullptr;

	SDL_Surface * rendering_surface = nullptr;

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

	void render_call();

	void set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect);
			
	qSceneObject * register_render_object(qRenderResource * non_owner, bool is_visible = true);

	RenderObject2D * get_render_object(int id);
	
	void set_scene_manager(SceneManager2D * sm) { scene_manager = sm; }
};

#endif // RENDER_MANAGER_TWO_D_H
