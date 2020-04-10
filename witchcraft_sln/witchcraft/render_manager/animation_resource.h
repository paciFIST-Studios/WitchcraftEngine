
#ifndef ANIMATION_RESOURCE_H
#define ANIMATION_RESOURCE_H

#include "../resource_manager/resource.h"
#include "../render_manager/2d_sprite_animation.h"

class AnimationResource : public qResource
{
public:

	Animation2D animation;

	AnimationResource() {}
	AnimationResource(Animation2D anim) : animation(anim) {}
	~AnimationResource() override {}
	void load() override {}
	void unload() override {}

};
#endif // !ANIMATION_RESOURCE_H

