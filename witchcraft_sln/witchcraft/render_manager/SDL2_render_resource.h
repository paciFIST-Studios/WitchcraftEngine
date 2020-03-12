#ifndef SDL2_RENDER_RESOURCE_H
#define SDL2_RENDER_RESOURCE_H

#include <Windows.h>

#include <SDL.h>

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
	void load();
	void unload();

	bool is_loaded() { return _is_loaded; }
};

#endif // SDL2_RENDER_RESOURCE_H