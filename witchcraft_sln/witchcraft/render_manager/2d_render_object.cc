#include "2d_render_object.h"

c2DRenderObject::c2DRenderObject()
	: _render_resource(nullptr)
	, _render_rect{uninit::UCHAR, uninit::UCHAR, uninit::UCHAR, uninit::UCHAR}
	, _position_x(uninit::FLOAT)
	, _position_y(uninit::FLOAT)
	, _is_visible(uninit::BOOL)
	, _color_key{uninit::UCHAR, uninit::UCHAR, uninit::UCHAR, uninit::UCHAR}
	, _color_key_is_enabled(uninit::BOOL)
{}

void c2DRenderObject::set_resource_object(cRenderResource * renderResource)
{
	if(renderResource)
	{
		_render_resource = renderResource;
		_render_rect.w = _render_resource->_surface->w;
		_render_rect.h = _render_resource->_surface->h;

		if (_color_key_is_enabled)
		{
			Uint32 color_key = SDL_MapRGB(
				_render_resource->_surface->format
				, _color_key.r
				, _color_key.g
				, _color_key.b
			);

			SDL_SetColorKey(_render_resource->_surface, SDL_TRUE, color_key);
		}
	}
}

POSITION_TUPLE c2DRenderObject::get_position()
{
	return std::make_tuple(_position_x, _position_y);
}

void c2DRenderObject::set_position(float x, float y)
{
	_position_x = x;
	_position_y = y;
}

bool c2DRenderObject::is_visible()
{
	return _is_visible;
}

void c2DRenderObject::set_is_visible(bool is)
{
	_is_visible = is;
}

SDL_Color c2DRenderObject::get_color_key()
{
	return _color_key;
}

void c2DRenderObject::set_color_key(SDL_Color key)
{
	_color_key = key;
}

bool c2DRenderObject::is_color_key_enabled()
{
	return _color_key_is_enabled;
}

void c2DRenderObject::set_color_key_enabled(bool is)
{
	_color_key_is_enabled = is;
}
