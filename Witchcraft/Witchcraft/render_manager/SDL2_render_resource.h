#ifndef SDL2_RENDER_RESOURCE_H
#define SDL2_RENDER_RESOURCE_H

#include <Windows.h>

#include <SDL.h>

#include "../resource_manager/resource.h"

// Represents a graphics resource, and is maintained 
// by its resource manager
class cRenderResource : public cResource
{
private:
protected:
public:
	SDL_Surface * _sdl_surface;

	cRenderResource();
	~cRenderResource();
	void load();
	void unload();
};

#endif // SDL2_RENDER_RESOURCE_H