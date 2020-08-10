#ifndef RENDER_RESOURCE_H
#define RENDER_RESOURCE_H

#include <map>
#include <tuple>

// SDL
#include <SDL.h>
#include <SDL_image.h>

// witchcraft
#include "../utility/utility.h"
#include "../resource_manager/resource.h"
#include "../render_manager/sprite_animation_2d.h"


// Render == "Graphical" == "Graphics"
class qRenderResource : public qResource
{
private:
protected:
	unsigned int surface_width = 0;
	unsigned int surface_height = 0;
	
	unsigned int tile_width = 0;
	unsigned int tile_height = 0;

	std::map<std::string, Animation2D> animations;
	std::string current_animation;

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

	std::tuple<int, int> get_width_height() const
	{
		return std::make_tuple(surface_width, surface_height);
	}

	void set_atlas_tiling(unsigned int width, unsigned int height)
	{
		tile_width = width;
		tile_height = height;
	}

	void add_animation(std::string const & name, Animation2D const & anim)
	{ 
		animations.insert(std::pair<std::string, Animation2D>(name, anim));
	}
	
	//void set_current_animation(std::string const & name)
	//{
	//	auto anim = &animations.find(name);
	//	if (anim)
	//	{
	//		current_animation = name;
	//	} // else error?
	//}

	std::vector<std::string> get_animation_list()
	{
		std::vector<std::string> result;

		for (auto&& pair : animations)
		{
			result.push_back(pair.first);
		}

		return result;
	}

	// std::vector<std::string> get_animations()
	// SDL_Rect get_next_animation_frame()
};

#endif // RENDER_RESOURCE_H
