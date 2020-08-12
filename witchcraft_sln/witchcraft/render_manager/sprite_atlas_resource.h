#ifndef SPRITE_ATLAS_RESOURCE_H
#define SPRITE_ATLAS_RESOURCE_H

#include <map>

#include "../render_manager/render_resource.h"

class SpriteAtlasResource : public qRenderResource
{
private:
protected:

	unsigned int sprite_width;
	unsigned int sprite_height;

	std::map<std::string, Animation2D> animations;
	std::string current_animation;

	SDL_Rect calculate_renderable_rect(unsigned int idx)
	{
		int columns = surface_width / sprite_width;
		int row = idx / columns;
		int col = idx % columns;

		SDL_Rect rect;
		rect.x = (col * sprite_width);
		rect.y = (row * sprite_height);
		rect.w = (col * sprite_width) + sprite_width;
		rect.h = (row * sprite_height) + sprite_height;
		return rect;
	}

public:
	   
	// also init base class
	SpriteAtlasResource() : qRenderResource()
	{}

	SpriteAtlasResource(
		  unsigned int UUID
		, unsigned int scope
		, std::string const & file_name
		, unsigned int tile_width
		, unsigned int tile_height
	)
		// initializer list
		: qRenderResource(UUID, scope, file_name)
		, sprite_width(tile_width)
		, sprite_height(tile_height)
	{
	}



	void add_animation(std::string const & name, Animation2D const & anim)
	{
		animations.insert(std::pair<std::string, Animation2D>(name, anim));
	}

	void set_current_animation(std::string const & name)
	{
		// if the animation is present in the map
		auto anim = animations.find(name);
		if (anim != animations.end())
		{
			current_animation = anim->first;
			int index = animations[current_animation].get_next_index();			
			current_renderable_rect = calculate_renderable_rect(index);
		}
	}

	std::string const & get_current_animation() { return current_animation; }

	std::vector<std::string> get_animation_list()
	{
		std::vector<std::string> result;

		for (auto&& pair : animations)
		{
			result.push_back(pair.first);
		}

		return result;
	}

	SDL_Rect const & get_next_animation_frame()
	{
		unsigned int index = animations[current_animation].get_next_index();
		current_renderable_rect = calculate_renderable_rect(index);
		return current_renderable_rect;
	}
};



#endif // !SPRITE_ATLAS_RESOURCE_H
