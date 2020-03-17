#ifndef SDL2_RENDER_RESOURCE_H
#define SDL2_RENDER_RESOURCE_H

// system
#include <Windows.h>

// SDL
#include <SDL.h>
#include <SDL_image.h>

// witchcraft
#include "../utility/utility.h"
#include "../resource_manager/resource.h"


// Represents a graphics resource, and is maintained 
// by its resource manager
class cRenderResource : public cResource
{
private:
protected:
	bool _is_loaded;

public:
	SDL_Surface * _sdl_surface;

	cRenderResource();
	~cRenderResource();

	SDL_Texture * _sdl_texture;

	void load(SDL_Renderer const * renderer);
	void unload();

	bool is_loaded() { return _is_loaded; }
};

#endif // SDL2_RENDER_RESOURCE_H