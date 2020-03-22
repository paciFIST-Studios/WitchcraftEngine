#include "2d_render_object.h"

void c2DRenderObject::set_resource_object(cRenderResource * RenderResource)
{
	if(RenderResource)
	{
		_render_resource = RenderResource;
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