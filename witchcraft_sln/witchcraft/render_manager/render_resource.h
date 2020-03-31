#ifndef RENDER_RESOURCE_H
#define RENDER_RESOURCE_H

// SDL
#include <SDL.h>
#include <SDL_image.h>

// witchcraft
#include "../resource_manager/resource.h"

// Render == "Graphical" == "Graphics"
class cRenderResource : public qResource
{
private:
protected:
	bool _is_loaded = uninit::BOOL;


public:
	SDL_Surface * _surface = nullptr;
	SDL_Texture * _texture = nullptr;

	// provided by qResource
	~cRenderResource() override;
	void load() override;
	void unload() override;

	cRenderResource();
	cRenderResource(
		unsigned int ID
		, unsigned int scope
		, std::string const & file_name
		, bool load_now = false
	);

	bool is_loaded() const;
};

#endif // RENDER_RESOURCE_H
