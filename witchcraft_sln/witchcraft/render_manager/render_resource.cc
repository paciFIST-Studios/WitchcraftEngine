#include "render_resource.h"

cRenderResource::~cRenderResource()
{}

void cRenderResource::load()
{
	SDL_Renderer * renderer = nullptr;
	
	unload();
	
	// load image to temp buffer
	auto temp_surface = IMG_Load(_file_name.c_str());
	
	if (temp_surface)
	{
		_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
	
		// free old buffer
		SDL_FreeSurface(temp_surface);
	
		// if surface is loaded, mark render resource as loaded, else, as not loaded
		//is_loaded = _surface ? true : false;
		_is_loaded = _texture ? true : false;
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

cRenderResource::cRenderResource() 
	: _is_loaded(false)
{}

cRenderResource::cRenderResource(
	// args
	  unsigned int ID
	, unsigned int scope
	, std::string const & file_name)
	// initializer (containing: call to base class)
	: qResource(ID, scope, file_name, RESOURCE_TYPE::RESOURCE_GRAPHIC)
	, _is_loaded(false)
{}
