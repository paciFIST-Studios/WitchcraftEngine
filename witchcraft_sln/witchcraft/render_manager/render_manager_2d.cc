#include "render_manager_2d.h"

bool RenderManager2D::init(unsigned int xOffset, unsigned int yOffset, unsigned int Width, unsigned int Height, bool fullScreen, char const * WindowTitle)
{
	PLOGV << witchcraft::log_strings::sdl_start;
	screen_width = Width;
	screen_height = Height;


	int flags = 0;

	// use for setting flag options
	if (true)
	{
		// we shouldn't do joystick stuff in the renderer
		// but this is where we're initializing SDL right now
		//flags = flags | SDL_INIT_JOYSTICK;
		flags = flags | SDL_INIT_EVERYTHING;
	}

	// SDL_Init() returns 0 on success, and a negative number on error
	if (SDL_Init(flags) < 0)
	{
		PLOGF << witchcraft::log_strings::sdl_init_failure << "\nError: " << SDL_GetError();
		return false;
	}


	flags = 0;
	// use for setting SDL_Image flag options
	if(true)
	{
		flags = flags | IMG_INIT_JPG;
		flags = flags | IMG_INIT_PNG;
	}
	// not sure what this returns, actually
	int icode = IMG_Init(flags);
	if ((icode & flags) != flags)
	{
		// failed to init
		PLOGF << witchcraft::log_strings::sdl_init_failure 
			  << "\nError: Could not initialize PNG + JPG support!\n" 
			  << IMG_GetError();
		return false;
	}


	flags = 0;
	if (fullScreen)
	{
		flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	flags = flags | SDL_WINDOW_SHOWN;

	// NOTE: DO NOT use the flag SDL_WINDOW_OPENGL
	// sdl chooses the renderer, use the 
	PLOGD << witchcraft::log_strings::sdl_window_init;
	SDL_CreateWindowAndRenderer(
		  Width
		, Height
		, static_cast<SDL_WindowFlags>(flags)
		, &program_window
		, &active_renderer
	);

	if (program_window == NULL)
	{
		PLOGF << witchcraft::log_strings::sdl_window_init_failure << "\nError: " << SDL_GetError();
		return false;
	}

	SDL_SetWindowTitle(program_window, WindowTitle);

	rendering_surface = SDL_GetWindowSurface(program_window);
	PLOGV << witchcraft::log_strings::sdl_window_init_success;

	return true;
}

bool RenderManager2D::update()
{
	SDL_RenderClear(active_renderer);
	render_call();
	SDL_RenderPresent(active_renderer);
	return true;
}

void RenderManager2D::shutdown()
{
	PLOGV << witchcraft::log_strings::sdl_begin_shutdown;
	IMG_Quit();
	SDL_DestroyWindow(program_window);
	SDL_FreeSurface(rendering_surface);
	SDL_DestroyRenderer(active_renderer);
	SDL_Quit();
	PLOGV << witchcraft::log_strings::sdl_stop;
}

void RenderManager2D::render_call()
{
	if (scene_manager == nullptr)
		return;

	auto layers = scene_manager->get_layers_ptrs_vector();
	for (auto&& layer : layers)
	{
		if (layer == nullptr)
			continue;

		if (layer->get_is_visible() == false)
			continue;

		auto objects = layer->get_layer_objects();

		for (auto&& obj : objects)
		{
			if (obj == nullptr)
				continue;

			if (obj->is_visible() == false)
				continue;

			// NOTE: we should move this out of the rendering area
			//// is this where the tick for the object is called? Is that okay?
			//obj->update();

			SDL_Rect dest;
			auto layer_pos = layer->get_offset();
			auto obj_pos = obj->get_position();
		
			dest.x = int(std::get<0>(layer_pos) + std::get<0>(obj_pos));
			dest.y = int(std::get<1>(layer_pos) + std::get<1>(obj_pos));
			
			auto scale = obj->get_scale();
			dest.w = int(obj->render_rect.w * std::get<0>(scale));
			dest.h = int(obj->render_rect.h * std::get<1>(scale));

			SDL_RenderCopy(
				  active_renderer
				, obj->render_resource->texture
				, &obj->render_rect
				, &dest
			);
		}
	}
}

void RenderManager2D::set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect)
{
	r = utility::clamp_to_0_255(r);
	g = utility::clamp_to_0_255(g);
	b = utility::clamp_to_0_255(b);

	SDL_FillRect(rendering_surface, rect, SDL_MapRGB(rendering_surface->format, r, g, b));
	SDL_UpdateWindowSurface(program_window);
}

qSceneObject * RenderManager2D::register_render_object(qRenderResource * non_owner, bool is_visible)
{
	std::unique_ptr<RenderObject2D> render_object = std::make_unique<qSceneObject>();
	render_object->set_is_visible(is_visible);
	render_object->set_render_resource(non_owner);

	// This SDL_Rect covers the parts of the texture we will display
	// We're sizing it to include the entire texture (ie: no sprite atlas support)
	auto wh = non_owner->get_width_height();
	render_object->render_rect.w = std::get<0>(wh);
	render_object->render_rect.h = std::get<1>(wh);
	render_object->render_rect.x = 0;
	render_object->render_rect.y = 0;

	auto result = static_cast<qSceneObject*>(render_object.get());
	render_objects.push_back(std::move(render_object));
	return result;
}

RenderObject2D * RenderManager2D::get_render_object(int id)
{
	for (auto&& object : render_objects)
	{
		if (object->render_resource->get_resource_id() == id)
			return object.get();
	}

	return nullptr;
}
