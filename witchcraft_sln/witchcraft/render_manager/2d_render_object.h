#ifndef TWO_D_RENDER_OBJECT_H
#define TWO_D_RENDER_OBJECT_H

#include <tuple>

#include <SDL_rect.h>

#include "../uninitialized_values.h"

#include "../render_manager/render_resource.h"
#include "../engine/engine.h"

#define POSITION_TUPLE std::tuple<float, float>

// 2D render object; each instance represents a unique renderable item
// in a 2d scene. It contains position, scale, & rotation, and these
// chearacteristics will allow the render manager to perform culling.
// It is possible many objects reference this instance (object pooling) 
class c2DRenderObject : public cEngineObject
{
private:
protected:

	float _position_x;
	float _position_y;

	bool _is_visible;

	SDL_Color _color_key;
	bool _color_key_is_enabled;

public:
	cRenderResource * _render_resource;
	SDL_Rect _render_rect;

	c2DRenderObject();

	void set_resource_object(cRenderResource * renderResource);

	// position
	POSITION_TUPLE get_position();
	void set_position(float x, float y);

	// visibility
	bool is_visible();
	void set_is_visible(bool is);

	// transparency color
	SDL_Color get_color_key();
	void set_color_key(SDL_Color key);
	bool is_color_key_enabled();
	void set_color_key_enabled(bool is);

	virtual void update() {};
};

#endif // !TWO_D_RENDER_OBJECT_H
