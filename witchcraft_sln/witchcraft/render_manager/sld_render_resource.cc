#include "sdl_render_resource.h"

void SDLRenderResource::attempt_load(std::string const & file_name)
{
	if (false == utility::file_exists(file_name))
	{
		PLOGE << "WARNING: FILE DOES NOT EXIST\tfile: \"" << file_name << "\"";
	}

	if (renderer == nullptr)
	{
		PLOGW << "WARNING: Loading texture with a null renderer.  Cannot show file: \"" << file_name << "\"";
	}

	//texture = IMG_LoadTexture(renderer, FILE.c_str());

	// this requires the c_str
	surface = IMG_Load(file_name.c_str());

	if (surface)
	{
		surface_width = surface->w;
		surface_height = surface->h;

		sdl_texture = SDL_CreateTextureFromSurface(renderer, surface);

		// NOTE: this shouldn't change in a RenderableResource
		// NOTE2: SpriteAtlasResrouce does change it
		current_renderable_rect.x = 0;
		current_renderable_rect.y = 0;
		current_renderable_rect.w = surface_width;
		current_renderable_rect.h = surface_height;
	}
}

SDLRenderResource::~SDLRenderResource()
{}

void SDLRenderResource::load()
{
	unload();
	attempt_load(filepath);
}

void SDLRenderResource::unload()
{
	if (sdl_texture)
	{
		SDL_DestroyTexture(sdl_texture);
		sdl_texture = nullptr;
	}

	if (surface)
	{
		SDL_FreeSurface(surface);
		surface = nullptr;
	}

}

void SDLRenderResource::bind_renderer(SDL_Renderer * renderer)
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

SDLRenderResource::SDLRenderResource() 
: current_renderable_rect({ uninit::UINT, uninit::UINT, uninit::UINT, uninit::UINT })
{}

SDLRenderResource::SDLRenderResource(
	  std::string const & name
	, std::string const & filepath
	, unsigned int scope)
	// -- end args
	: EngineResourceBase(name , filepath, EResourceType::IMAGE, scope)
	, current_renderable_rect({ uninit::UINT, uninit::UINT, uninit::UINT, uninit::UINT })
	// -- end initializer
{
}

bool SDLRenderResource::is_loaded() const
{
	return (sdl_texture != nullptr);
}

bool SDLRenderResource::renderer_is_ready() const
{
	return (renderer != nullptr);
}
