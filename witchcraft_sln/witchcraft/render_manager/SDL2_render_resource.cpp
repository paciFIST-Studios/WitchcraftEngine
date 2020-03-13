#include "SDL2_render_resource.h"
#include "SDL2_render_resource.h"

void cRenderResource::load(SDL_Renderer * renderer)
{
	if (RenderManager == nullptr)
	{
		// throw error
	}

	// 
	unload();
	
	// load image to temp buffer
	size_t size = static_cast<size_t>(utility::file_size(_file_name.c_str()));
	SDL_Surface * temp_surface = IMG_Load(_file_name.c_str());

	if (tmp_surface)
	{
		_sdl_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);

		// create an optimized image buffer
		//_sdl_surface = SDL_DisplayFormat(temp_surface);

		// free old buffer
		SDL_FreeSurface(temp_surface);

		// if surface is loaded, mark render resource as loaded, else, as not loaded
		//is_loaded = _sdl_surface ? true : false;
		//is_loaded = _sdl_texture ? true : false;
	}
}

void cRenderResource::unload()
{
	if (_sdl_texture)
	{
		SDL_DestroyTexture(_sdl_texture);
		_sdl_texture = nullptr;

	}

	if (_sdl_surface)
	{
		SDL_FreeSurface(_sdl_surface);
		_sdl_surface = nullptr;
	}

	_is_loaded = false;
}
