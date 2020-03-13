#ifndef SDL_2D_RENDER_OBJECT_H
#define SDL_2D_RENDER_OBJECT_H


#include <SDL_rect.h>

#include "../render_manager/SDL2_render_resource.h"
#include "../engine/engine.h"

// 2D render object; each instance represents a unique renderable item
// in a 2d scene. It contains position, scale, & rotation, and these
// chearacteristics will allow the render manager to perform culling.
// It is possible many objects reference this instance (object pooling) 
class cSDL2DRenderObject : public cEngineObject
{
private:
protected:
public:
	cRenderResource * _render_resource;
	SDL_Rect _render_rect;
	float _position_x;
	float _position_y;

	bool _is_visible;

	SDL_Color _color_key;
	bool _color_key_is_enabled;

	cSDL2DRenderObject();

	void set_resource_object(cRenderResource * RenderResource);

	virtual void update() {};
};

#endif // !SDL_2D_RENDER_OBJECT_H


