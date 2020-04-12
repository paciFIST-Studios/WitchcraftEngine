#ifndef TWO_D_RENDER_OBJECT_H
#define TWO_D_RENDER_OBJECT_H

#include <tuple>

#include <SDL_rect.h>

#include "../uninitialized_values.h"

#include "../render_manager/render_resource.h"
#include "../engine/engine_object.h"

#define POSITION_TUPLE std::tuple<float, float>

// 2D render object; each instance represents a unique renderable item
// in a 2d scene. It contains position, scale, & rotation, and these
// characteristics will allow the render manager to perform culling.
class RenderObject2D : public qEngineObject
{
private:
protected:

	float position_x_;
	float position_y_;

	bool is_visible_;

	SDL_Color color_key_;
	bool color_key_is_enabled_;

public:
	qRenderResource * render_resource;
	SDL_Rect render_rect;

	RenderObject2D();

	void set_render_resource(qRenderResource * renderResource);

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
