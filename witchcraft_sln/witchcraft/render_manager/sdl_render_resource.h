#ifndef SDL_RENDER_RESOURCE_H
#define SDL_RENDER_RESOURCE_H

#include <tuple>

// SDL
#include <SDL.h>
#include <SDL_image.h>

// witchcraft
#include "../utility/utility.h"
#include "../resource_manager/engine_resource_base.h"
#include "../render_manager/sprite_animation_2d.h"


// Render == "Graphical" == "Graphics"
class SDLRenderResource : public EngineResourceBase
{
private:
protected:
	unsigned int surface_width = 0;
	unsigned int surface_height = 0;
	

	SDL_Renderer * renderer = nullptr;

	void attempt_load(std::string const & file_name);
	
	SDL_Rect current_renderable_rect;

public:
	SDL_Surface * surface = nullptr;
	SDL_Texture * sdl_texture = nullptr;

	// provided by EngineResourceBase
	~SDLRenderResource() override;
	void load() override;
	void unload() override;

	void bind_renderer(SDL_Renderer * renderer);

	SDLRenderResource();
	SDLRenderResource(
		  std::string const & name
		, std::string const & filepath
		, unsigned int scope
	);

	bool is_loaded() const;
	bool renderer_is_ready() const;

	SDL_Rect get_renderable_rect() const { return current_renderable_rect; }

	std::tuple<int, int> get_width_height() const
	{
		return std::make_tuple(surface_width, surface_height);
	}	
};

#endif // !SDL_RENDER_RESOURCE_H
