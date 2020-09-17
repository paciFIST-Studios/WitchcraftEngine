#include "render_object_2d.h"

RenderObject2D::RenderObject2D()
	: render_resource(nullptr)
	, position_x(uninit::FLOAT)
	, position_y(uninit::FLOAT)
	, scale_x(uninit::FLOAT)
	, scale_y(uninit::FLOAT)
	, visible(uninit::BOOL)
	, color_key{uninit::UCHAR, uninit::UCHAR, uninit::UCHAR, uninit::UCHAR}
	, color_key_enabled(uninit::BOOL)
{}

void RenderObject2D::set_render_resource(SDLRenderResource * resource)
{
	if(resource)
	{
		render_resource = resource;
		
		if (render_resource->surface)
		{
			// set scaling to 1:1
			scale_x = 1.0f;
			scale_y = 1.0f;
		}

		if (color_key_enabled)
		{
			if (render_resource->surface != nullptr)
			{
				Uint32 ck = SDL_MapRGB(
					render_resource->surface->format
					, color_key.r
					, color_key.g
					, color_key.b
				);

				SDL_SetColorKey(render_resource->surface, SDL_TRUE, ck);
			}
		}
	}
}

POSITION_TUPLE RenderObject2D::get_position() const
{
	return std::make_tuple(position_x, position_y);
}

void RenderObject2D::set_position(float x, float y)
{
	position_x = x;
	position_y = y;
}

SCALE_TUPLE RenderObject2D::get_scale() const
{
	return std::make_tuple(scale_x, scale_y);
}
void RenderObject2D::set_scale(float x, float y)
{
	scale_x = x;
	scale_y = y;
}

float RenderObject2D::get_rotation() const
{
	return rotation_radians;
}

void RenderObject2D::set_rotation(float radians)
{
	rotation_radians = radians;
}

bool RenderObject2D::is_visible() const
{
	return visible;
}

void RenderObject2D::set_is_visible(bool is)
{
	visible = is;
}

SDL_Color RenderObject2D::get_color_key() const
{
	return color_key;
}

void RenderObject2D::set_color_key(SDL_Color key)
{
	color_key = key;
}

bool RenderObject2D::is_color_key_enabled() const
{
	return color_key_enabled;
}

void RenderObject2D::set_color_key_enabled(bool is)
{
	color_key_enabled = is;
}
