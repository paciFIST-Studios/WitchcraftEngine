#include "render_resource.h"

void qRenderResource::attempt_load(std::string const & file_name)
{
	if (false == utility::file_exists(file_name))
	{
		PLOGE << "WARNING: FILE DOES NOT EXIST\n" << file_name;
	}

	if (renderer == nullptr)
	{
		PLOGW << "WARNING: Loading texture with a null renderer.  This image will not be shown: ";
		PLOGW << "\t\tfile_name: " << file_name;
	}

	texture = IMG_LoadTexture(renderer, file_name.c_str());

	surface = IMG_Load(file_name.c_str());
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
}

void qRenderResource::bind_renderer(SDL_Renderer * renderer)
{
	if (renderer != nullptr)
	{
		this->renderer = renderer;
	}
}

qRenderResource::qRenderResource() 
{}

qRenderResource::qRenderResource(
	  unsigned int ID
	, unsigned int scope
	, std::string const & file_name)
	// -- end args
	: qResource(ID, scope, file_name, RESOURCE_TYPE::RESOURCE_GRAPHIC)
	// -- end initializer
{
}

bool qRenderResource::is_loaded() const
{
	return (texture != nullptr);
}

bool qRenderResource::renderer_is_ready() const
{
	return (renderer != nullptr);
}
