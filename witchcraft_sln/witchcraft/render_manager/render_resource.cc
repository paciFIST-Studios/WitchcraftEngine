#include "SDL2_render_resource.h"
#include "SDL2_render_resource.h"

cRenderResource::~cRenderResource()
{
}

void cRenderResource::load()
{
	SDL_Renderer * renderer = nullptr;

	unload();
	
	// load image to temp buffer
	auto temp_surface = IMG_Load(_file_name.c_str());

	if (tmp_surface)
	{
		_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);

		// free old buffer
		SDL_FreeSurface(temp_surface);

		// if surface is loaded, mark render resource as loaded, else, as not loaded
		//is_loaded = _surface ? true : false;
		is_loaded = _texture ? true : false;
	}
}

void cRenderResource::unload()
{
	if (_texture)
	{
		SDL_DestroyTexture(_texture);
		_texture = nullptr;
	}

	if (_surface)
	{
		SDL_FreeSurface(_surface);
		_surface = nullptr;
	}

	_is_loaded = false;
}
