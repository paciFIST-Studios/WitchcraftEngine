#include "SDL2_2D_render_manager.h"


bool cSDL2RenderManager::init(unsigned int xOffset, unsigned int yOffset, unsigned int Width, unsigned int Height, bool fullScreen, char const * WindowTitle)
{
	std::cout << "\nSDL Initialization start";
	// SDL_Init() returns 0 on success, and a negative number on error
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "\nSDL initialization failure\n" << "Error: " << SDL_GetError();
		// log error: could not initialize SDL Video
		// SDL_QUIT()
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

	std::cout << "\nSDL Window Creation start";

	SDL_CreateWindowAndRenderer(
		  Width
		, Height
		, static_cast<SDL_WindowFlags>(flags)
		, &_window
		, &_renderer
	);

	if (_window == NULL)
	{
		std::cout << "\nSDL Window Creation FAILURE";
		return false;
	}

	SDL_SetWindowTitle(_window, WindowTitle);

	_rendering_surface = SDL_GetWindowSurface(_window);

	std::cout << "\nSDL Window Creation Success";
	return true;
}

void cSDL2RenderManager::shutdown()
{
	std::cout << "\nSDL Shutdown begin";
	SDL_DestroyWindow(_window);
	SDL_FreeSurface(_rendering_surface);
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
	std::cout << "\nSDL Shutdown complete";
}

void cSDL2RenderManager::set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect)
{
	r = utility::clamp_value_to_uint8(r);
	g = utility::clamp_value_to_uint8(g);
	b = utility::clamp_value_to_uint8(b);

	SDL_FillRect(_rendering_surface, rect, SDL_MapRGB(_rendering_surface->format, r, g, b));
	SDL_UpdateWindowSurface(_window);
}

bool cSDL2RenderManager::update()
{
	//std::cout << "\ncSDL2RenderManager::update()";

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		//std::cout << "\nEvent: " << event.type;

		// check messages
		switch (event.type)
		{
			case SDL_QUIT: { return false; }
					
			case SDL_KEYDOWN:
			{
				// [ESC]
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					return false;
				}
				// others
			}
	
		} //end switch
	} //end message

	// clear screen
	SDL_RenderClear(_renderer);
	//std::cout << "\nRenderer Cleared!";


	// paint new objects
	render_all_objects();
	//std::cout << "\nAll objects rendered!";

	// swap buffer
	SDL_RenderPresent(_renderer);
	//std::cout << "\nBuffer Swapped!";

	return true;
}

cResource * cSDL2RenderManager::load_resource_from_xml(XML::xml_node<> const * xml)
{
	if (xml == nullptr)
		return nullptr;
	
	cResource * resource = new cResource();
	resource->_type = RESOURCE_GRAPHIC;

	for (XML::xml_attribute<> * element_attribute = xml->first_attribute();
		element_attribute;
		element_attribute = element_attribute->next_attribute()
	)
	{
		std::string attribute_name = element_attribute->name();
		std::string attribute_value = element_attribute->value();

		if (attribute_name == "UID")
		{
			resource->_resource_id = atoi(attribute_value.c_str());
		}
		if (attribute_name == "filename")
		{
			resource->_file_name = attribute_value;
		}
		if (attribute_name == "scenescope")
		{
			resource->_scope = atoi(attribute_value.c_str());
		}
	}

	return resource;
}

void cSDL2RenderManager::render_all_objects()
{
	//std::cout << "\nNow rendering:";

	if (_render_objects.size() < 1)
	{
		//std::cout << "\nNo render objects in renderer!";
		return;
	}

	std::list<cSDL2DRenderObject*>::iterator list_iter;

	// loop all objects and blit them
	for (list_iter = _render_objects.begin(); list_iter != _render_objects.end(); list_iter++)
	{
		if ((*list_iter)->_is_visible == false)
			continue;

		std::cout << "\n\tobject id: " << (*list_iter)->_id;

		(*list_iter)->update();
		SDL_Rect position;
		position.x = int((*list_iter)->_position_x);
		position.y = int((*list_iter)->_position_y);

		SDL_RenderCopy(
			  _renderer
			, (*list_iter)->_render_resource->_sdl_texture
			, &(*list_iter)->_render_rect
			, &position
		);
	}

}
