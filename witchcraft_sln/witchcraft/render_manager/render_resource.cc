#include "render_resource.h"

void qRenderResource::attempt_load(std::string const & file_name)
{
	if (false == utility::file_exists(file_name))
	{
		PLOGE << "WARNING: FILE DOES NOT EXIST\n" << file_name;
	}

	surface = IMG_Load(file_name.c_str());

	SDL_Renderer * renderer = nullptr;
	
	// load image to temp buffer
	if (surface)
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);
	
		// if surface is loaded, mark render resource as loaded, else, as not loaded
		//is_loaded = surface ? true : false;
		is_loaded_ = texture ? true : false;
	}

	if (surface != nullptr)
	{
		is_loaded_ = true;
	}	
}

qRenderResource::~qRenderResource()
{}

void qRenderResource::load()
{
	unload();
	attempt_load(_file_name.c_str());
}

void qRenderResource::unload()
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
	
	if (surface)
	{
		SDL_FreeSurface(surface);
		surface = nullptr;
	}
	
	is_loaded_ = false;
}

qRenderResource::qRenderResource() 
	: is_loaded_(false)
{}

qRenderResource::qRenderResource(
	  unsigned int ID
	, unsigned int scope
	, std::string const & file_name
	, bool load_now)
	// -- end args
	: qResource(ID, scope, file_name, RESOURCE_TYPE::RESOURCE_GRAPHIC)
	, is_loaded_(false)
	// -- end initializer
{
	if (load_now)
	{
		attempt_load(file_name);
	}
}

bool qRenderResource::is_loaded() const
{
	return is_loaded_;
}
