#ifndef RENDER_RESOURCE_H
#define RENDER_RESOURCE_H

// SDL
#include <SDL.h>
#include <SDL_image.h>

// witchcraft
#include "../utility/utility.h"
#include "../resource_manager/resource.h"

// Render == "Graphical" == "Graphics"
class qRenderResource : public qResource
{
private:
protected:
	SDL_Renderer * renderer = nullptr;

	void attempt_load(std::string const & file_name);
	
public:
	SDL_Surface * surface = nullptr;
	SDL_Texture * texture = nullptr;

	// provided by qResource
	~qRenderResource() override;
	void load() override;
	void unload() override;

	void bind_renderer(SDL_Renderer * renderer);

	qRenderResource();
	qRenderResource(
		unsigned int ID
		, unsigned int scope
		, std::string const & file_name
	);

	bool is_loaded() const;
	bool renderer_is_ready() const;
};

#endif // RENDER_RESOURCE_H
