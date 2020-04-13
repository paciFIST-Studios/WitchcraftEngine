#include "render_object_2d.h"

RenderObject2D::RenderObject2D()
	: render_resource(nullptr)
	, render_rect{uninit::UCHAR, uninit::UCHAR, uninit::UCHAR, uninit::UCHAR}
	, position_x(uninit::FLOAT)
	, position_y(uninit::FLOAT)
	, visible(uninit::BOOL)
	, color_key{uninit::UCHAR, uninit::UCHAR, uninit::UCHAR, uninit::UCHAR}
	, color_key_enabled(uninit::BOOL)
{}

void RenderObject2D::set_render_resource(qRenderResource * resource)
{
	if(resource)
	{
		render_resource = resource;
		
		if (render_resource->surface)
		{
			render_rect.w = render_resource->surface->w;
			render_rect.h = render_resource->surface->h;
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

POSITION_TUPLE RenderObject2D::get_position()
{
	return std::make_tuple(position_x, position_y);
}

void RenderObject2D::set_position(float x, float y)
{
	position_x = x;
	position_y = y;
}

bool RenderObject2D::is_visible()
{
	return visible;
}

void RenderObject2D::set_is_visible(bool is)
{
	visible = is;
}

SDL_Color RenderObject2D::get_color_key()
{
	return color_key;
}

void RenderObject2D::set_color_key(SDL_Color key)
{
	color_key = key;
}

bool RenderObject2D::is_color_key_enabled()
{
	return color_key_enabled;
}

void RenderObject2D::set_color_key_enabled(bool is)
{
	color_key_enabled = is;
}
