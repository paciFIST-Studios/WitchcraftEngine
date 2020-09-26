#ifndef RENDER_OBJECT_TWO_D_H
#define RENDER_OBJECT_TWO_D_H

#include <tuple>

#include <SDL_rect.h>

#include "../uninitialized_values.h"

#include "../render_manager/sdl_render_resource.h"
#include "../engine/engine_object.h"

#define POSITION_TUPLE std::tuple<float, float>
#define SCALE_TUPLE std::tuple<float, float>

// 2D render object; each instance represents a unique renderable item
// in a 2d scene. It contains position, scale, & rotation, and these
// characteristics will allow the render manager to perform culling.
class RenderObject2D : public EngineObjectBase
{
private:
protected:

	float position_x;
	float position_y;

	float scale_x;
	float scale_y;

	float rotation_radians;

	bool visible;

	SDL_Color color_key;
	bool color_key_enabled;

public:
	SDLRenderResource * render_resource;

	RenderObject2D();

	void set_render_resource(SDLRenderResource * renderResource);

	// position
	POSITION_TUPLE get_position() const;
	void set_position(float x, float y);

	// scale
	SCALE_TUPLE get_scale() const;
	void set_scale(float x, float y);

	float get_rotation() const;
	void set_rotation(float radians);

	// visibility
	bool is_visible() const;
	void set_is_visible(bool is);

	// transparency color
	SDL_Color get_color_key() const;
	void set_color_key(SDL_Color key);
	bool is_color_key_enabled() const;
	void set_color_key_enabled(bool is);

	virtual void update() {};
};

#endif // !RENDER_OBJECT_TWO_D_H
