
#ifndef ANIMATION_RESOURCE_H
#define ANIMATION_RESOURCE_H

#include "../resource_manager/resource.h"
#include "../render_manager/2d_sprite_animation.h"

class cAnimationResource : public cResource
{
public:

	c2DSpriteAnimation animation;

	cAnimationResource() {}
	cAnimationResource(c2DSpriteAnimation anim) : animation(anim) {}
	~cAnimationResource() override {}
	void load() override {}
	void unload() override {}

};
#endif // !ANIMATION_RESOURCE_H

