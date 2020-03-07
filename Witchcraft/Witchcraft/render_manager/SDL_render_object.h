#ifndef SDL_RENDER_OBJECT_H
#define SDL_RENDER_OBJECT_H


#include <SDL_rect.h>
#include <SDL_Image>

#include "../render_manager/SDL2_render_resource.h"
#include "../engine/engine.h"

// 2D render object; each instance represents a unique renderable item
// in a 2d scene. It contains position, scale, & rotation, and these
// chearacteristics will allow the render manager to perform culling.
// It is possible many objects reference this instance (object pooling) 
class cSDLRenderObject : public cEngineObject
{
private:
protected:
public:
	cRenderResource * _render_resource;
	SDL_Rect _render_rect;
	float _pos_x;
	float _pos_y;

	bool _is_visible;

	SDL_Color _color_key;
	bool _color_key_is_enabled;

	cSDLRenderObject();

	void set_resoruce_object(cRenderResource * RenderResource);

	virtual void update();
};

#endif // !SDL_RENDER_OBJECT_H


