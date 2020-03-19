#ifndef RENDER_RESOURCE_H
#define RENDER_RESOURCE_H

// SDL
#include <SDL.h>
#include <SDL_image.h>

// witchcraft
#include "../resource_manager/resource.h"

// Render == "Graphical" == "Graphics"
class cRenderResource : public cResource
{
private:
protected:
	bool _is_loaded;
	SDL_Surface * _surface;
	SDL_Texture * _texture;

public:
	// provided by cResource
	~cRenderResource() override;
	void load() override;
	void unload() override;

	// not virtual
	cRenderResource();
	cRenderResource(unsigned int ID, unsigned int scope, std::string const & fileName);
	bool is_loaded() { return _is_loaded; }
};

#endif // RENDER_RESOURCE_H
