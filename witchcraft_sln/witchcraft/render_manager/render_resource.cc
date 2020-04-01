#include "render_resource.h"

void cRenderResource::attempt_load(std::string const & file_name)
{
	if (false == utility::file_exists(file_name))
	{
		PLOGE << "WARNING: FILE DOES NOT EXIST\n" << file_name;
	}

	_surface = IMG_Load(file_name.c_str());

	//SDL_Renderer * renderer = nullptr;
	//
	//// load image to temp buffer
	//auto temp_surface = IMG_Load(_file_name.c_str());
	//if (temp_surface)
	//{
	//	_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
	//
	//	// free old buffer
	//	SDL_FreeSurface(temp_surface);
	//
	//	// if surface is loaded, mark render resource as loaded, else, as not loaded
	//	//is_loaded = _surface ? true : false;
	//	_is_loaded = _texture ? true : false;
	//}

	if (_surface != nullptr)
	{
		_is_loaded = true;
	}	
}

cRenderResource::~cRenderResource()
{}

void cRenderResource::load()
{
	unload();
	attempt_load(_file_name.c_str());

	//SDL_Renderer * renderer = nullptr;
	//
	//unload();
	//
	//// load image to temp buffer
	//auto temp_surface = IMG_Load(_file_name.c_str());
	//
	//if (temp_surface)
	//{
	//	_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
	//
	//	// free old buffer
	//	SDL_FreeSurface(temp_surface);
	//
	//	// if surface is loaded, mark render resource as loaded, else, as not loaded
	//	//is_loaded = _surface ? true : false;
	//	_is_loaded = _texture ? true : false;
	//}
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
	  unsigned int ID
	, unsigned int scope
	, std::string const & file_name
	, bool load_now)
	// -- end args
	: qResource(ID, scope, file_name, RESOURCE_TYPE::RESOURCE_GRAPHIC)
	, _is_loaded(false)
	// -- end initializer
{
	if (load_now)
	{
		attempt_load(file_name);
	}
}

bool cRenderResource::is_loaded() const
{
	return _is_loaded;
}
