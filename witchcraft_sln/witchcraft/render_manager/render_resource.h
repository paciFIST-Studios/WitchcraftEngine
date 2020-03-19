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

// Render == "Graphical" == "Graphics"
class cRenderResource : public cResource
{
private:
protected:
	bool _is_loaded;

public:
	SDL_Surface * _surface;
	SDL_Texture * _texture;

	// provided by cResource
	~cRenderResource() override;
	void load() override;
	void unload() override;

	inline cRenderResource() 
		: _is_loaded(false)
	{}

	bool is_loaded() { return _is_loaded; }
};

#endif // SDL2_RENDER_RESOURCE_H
