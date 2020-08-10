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
		auto anim = &animations.find(name);
		if (anim)
		{
			current_animation = name;

			// set renderable area, based on current animation
		} // else error?
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

	// SDL_Rect get_next_animation_frame()
};



#endif // !SPRITE_ATLAS_RESOURCE_H
