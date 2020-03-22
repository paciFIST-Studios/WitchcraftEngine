#include "2d_render_manager.h"

bool c2DRenderManager::init(unsigned int xOffset, unsigned int yOffset, unsigned int Width, unsigned int Height, bool fullScreen, char const * WindowTitle)
{
	PLOGV << witchcraft::log_strings::sdl_start;
	// SDL_Init() returns 0 on success, and a negative number on error
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		PLOGF << witchcraft::log_strings::sdl_init_failure << "\nError: " << SDL_GetError();
		return false;
	}
	
	// Flags are set with bitmasking
	// NOTE: DO NOT use the flag SDL_WINDOW_OPENGL
	// sdl chooses the renderer, use the 
	int flags = 0;

	if (fullScreen)
	{
		flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	flags = flags | SDL_WINDOW_SHOWN;

	PLOGD << witchcraft::log_strings::sdl_window_init;
	SDL_CreateWindowAndRenderer(
		  Width
		, Height
		, static_cast<SDL_WindowFlags>(flags)
		, &_window
		, &_current_renderer
	);

	if (_window == NULL)
	{
		PLOGF << witchcraft::log_strings::sdl_window_init_failure << "\nError: " << SDL_GetError();
		return false;
	}

	SDL_SetWindowTitle(_window, WindowTitle);

	_rendering_surface = SDL_GetWindowSurface(_window);
	PLOGV << witchcraft::log_strings::sdl_window_init_success;

	return true;
}

void c2DRenderManager::shutdown()
{	
	PLOGV << witchcraft::log_strings::sdl_begin_shutdown;
	SDL_DestroyWindow(_window);
	SDL_FreeSurface(_rendering_surface);
	SDL_DestroyRenderer(_current_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
	PLOGV << witchcraft::log_strings::sdl_stop;
}

void c2DRenderManager::set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect)
{
	r = utility::clamp_value_to_uint8(r);
	g = utility::clamp_value_to_uint8(g);
	b = utility::clamp_value_to_uint8(b);

	SDL_FillRect(_rendering_surface, rect, SDL_MapRGB(_rendering_surface->format, r, g, b));
	SDL_UpdateWindowSurface(_window);
}

bool c2DRenderManager::update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// check messages
		switch (event.type)
		{
			case SDL_QUIT: { return false; }
					
			case SDL_KEYDOWN:
			{
				// [ESC]
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					PLOGI << witchcraft::log_strings::sdl_break_event_polling;
					return false;
				}
				// others
			}
	
		} //end switch
	} //end message

	// clear screen
	SDL_RenderClear(_current_renderer);
	render_all_objects();
	SDL_RenderPresent(_current_renderer);

	return true;
}

std::unique_ptr<cResource> c2DRenderManager::load_resource_from_xml(XML::xml_node<> const & xml)
{	
	// some default values
	unsigned int	resource_id		= cResource::UNINIT_RESOURCE_ID;
	unsigned int	resource_scope	= cResource::UNINIT_RESOURCE_SCOPE;
	std::string		file_name		= cResource::UNINIT_FILE_NAME;
	
	for (XML::xml_attribute<> * element_attribute = xml.first_attribute();
		element_attribute;
		element_attribute = element_attribute->next_attribute()
	)
	{
		std::string attribute_name = element_attribute->name();
		std::string attribute_value = element_attribute->value();

		if (attribute_name == "UID")
		{
			// atoi stands for ASCII-to-integer, and is used for
			// parsing a string to an int
			resource_id = atoi(attribute_value.c_str());
		}
		else if (attribute_name == "filename")
		{
			file_name = attribute_value;
		}
		else if (attribute_name == "scenescope")
		{
			resource_scope = atoi(attribute_value.c_str());
		}
	}

	PLOGV << witchcraft::log_strings::resource_manager_meta_load << file_name;

	// NOTE: We're making a cRenderResource, and then moving it to a cResource ptr
	std::unique_ptr<cResource> resource = std::make_unique<cRenderResource>(
			  resource_id
			, resource_scope
			, file_name
		);

	return std::move(resource);
}

void c2DRenderManager::render_all_objects()
{
	if (_render_objects.size() < 1)
		return;

	for (auto&& object : _render_objects)
	{
		if (object->_is_visible == false)
			continue;

		object->update();
		SDL_Rect position;
		position.x = int(object->_position_x);
		position.y = int(object->_position_y);

		SDL_RenderCopy(
			  _current_renderer
			, object->_render_resource->_texture
			, &object->_render_rect
			, &position
		);
	}
}