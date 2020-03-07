#ifndef SDL2_RENDER_MANAGER_H
#define SDL2_RENDER_MANAGER_H

#include <Windows.h>

#include <SDL.h>

//#include "2D_render_manager.h"
#include "../resource_manager/resource_manager.h"

// Represents a graphics resource, and is maintained 
// by its resource manager
class cRenderResource : public cResource
{
private:
protected:
public:
	SDL_Surface * _surface;

	cRenderResource();
	~cRenderResource();
	void load();
	void unload();
};

#endif // SDL2_RENDER_MANAGER_H