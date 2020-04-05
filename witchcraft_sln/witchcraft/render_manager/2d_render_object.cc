#include "2d_render_object.h"

c2DRenderObject::c2DRenderObject()
	: render_resource(nullptr)
	, render_rect{uninit::UCHAR, uninit::UCHAR, uninit::UCHAR, uninit::UCHAR}
	, position_x_(uninit::FLOAT)
	, position_y_(uninit::FLOAT)
	, is_visible_(uninit::BOOL)
	, color_key_{uninit::UCHAR, uninit::UCHAR, uninit::UCHAR, uninit::UCHAR}
	, color_key_is_enabled_(uninit::BOOL)
{}

void c2DRenderObject::set_resource_object(qRenderResource * renderResource)
{
	if(renderResource)
	{
		render_resource = renderResource;
		render_rect.w = render_resource->surface->w;
		render_rect.h = render_resource->surface->h;

		if (color_key_is_enabled_)
		{
			Uint32 color_key = SDL_MapRGB(
				render_resource->surface->format
				, color_key_.r
				, color_key_.g
				, color_key_.b
			);

			SDL_SetColorKey(render_resource->surface, SDL_TRUE, color_key);
		}
	}
}

POSITION_TUPLE c2DRenderObject::get_position()
{
	return std::make_tuple(position_x_, position_y_);
}

void c2DRenderObject::set_position(float x, float y)
{
	position_x_ = x;
	position_y_ = y;
}

bool c2DRenderObject::is_visible()
{
	return is_visible_;
}

void c2DRenderObject::set_is_visible(bool is)
{
	is_visible_ = is;
}

SDL_Color c2DRenderObject::get_color_key()
{
	return color_key_;
}

void c2DRenderObject::set_color_key(SDL_Color key)
{
	color_key_ = key;
}

bool c2DRenderObject::is_color_key_enabled()
{
	return color_key_is_enabled_;
}

void c2DRenderObject::set_color_key_enabled(bool is)
{
	color_key_is_enabled_ = is;
}
