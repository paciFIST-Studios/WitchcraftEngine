#include "render_resource.h"

void qRenderResource::attempt_load(std::string const & file_name)
{
	if (false == utility::file_exists(file_name))
	{
		PLOGE << "WARNING: FILE DOES NOT EXIST";
		PLOGE << "\t\tfile_name: " << file_name;
	}

	if (renderer == nullptr)
	{
		PLOGW << "WARNING: Loading texture with a null renderer.  This image will not be shown: ";
		PLOGW << "\t\tfile_name: " << file_name;
	}

	//texture = IMG_LoadTexture(renderer, file_name.c_str());

	// this requires the c_str
	surface = IMG_Load(file_name.c_str());

	if (surface)
	{
		surface_width = surface->w;
		surface_height = surface->h;

		texture = SDL_CreateTextureFromSurface(renderer, surface);

		// NOTE: this shouldn't change in a RenderableResource
		// NOTE2: SpriteAtlasResrouce does change it
		current_renderable_rect.x = 0;
		current_renderable_rect.y = 0;
		current_renderable_rect.w = surface_width;
		current_renderable_rect.h = surface_height;
	}
}

qRenderResource::~qRenderResource()
{}

void qRenderResource::load()
{
	unload();
	attempt_load(_file_name);
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

}

void qRenderResource::bind_renderer(SDL_Renderer * renderer)
{
	if (this == nullptr)
	{
		PLOGE << "WARNING: RENDER RESOURCE IS NOT FULLY INITIALIZED";
		// todo: print callstack so we know what happened here
		return;
	}

	if (renderer != nullptr)
	{
		this->renderer = renderer;
	}
}

qRenderResource::qRenderResource() 
: current_renderable_rect({ uninit::UINT, uninit::UINT, uninit::UINT, uninit::UINT })
{}

qRenderResource::qRenderResource(
	  unsigned int ID
	, unsigned int scope
	, std::string const & file_name)
	// -- end args
	: qResource(ID, scope, file_name, EResourceType::RESOURCE_GRAPHIC)
	, current_renderable_rect({ uninit::UINT, uninit::UINT, uninit::UINT, uninit::UINT })
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
