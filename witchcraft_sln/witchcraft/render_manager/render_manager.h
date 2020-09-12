#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

// stdlib
#include <iomanip>
#include <iostream>
#include <list>
#include <math.h>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <time.h>

// sdl
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <GL/glew.h>

// rapidxml
#include "../../lib/rapidxml/rapidxml.hpp"
#include "../../lib/rapidxml/rapidxml_utils.hpp"
#define XML rapidxml

// imgui
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl.h"
#include "../imgui/imgui_impl_opengl3.h"


// witchcraft
#include "../engine/engine_object.h"

#include "../resource_manager/resource.h"
#include "../string_constants.h"
#include "../utility/utility.h"
#include "render_object_2d.h"
#include "render_resource.h"

#include "../scene_manager/scene_manager_2d.h"
#include "../scene_manager/scene_object.h"

#include "opengl_shader.h"

#include "../console/console.h"

class SceneManager2D;
class qSceneObject;
class Console;

enum class ERendererState : unsigned char
{
	  UNINIT		= 0x01
	, CONSTRUCTED	= 0x02
	, SDL_INIT_OK	= 0x04
	, OPENGL_INIT_OK= 0x08
	, UPDATE_OK		= 0x10
	, SHUTDOWN_START= 0x20
	, SDL_QUIT_OK	= 0x40
	, SHUTDOWN_OK	= 0x80
};


class RenderManager : public qEngineObject
{
public:
	typedef std::vector<std::unique_ptr<RenderObject2D>> RenderObjectsVector;

private:
	ERendererState renderer_state = ERendererState::UNINIT;

	std::map<GLenum, std::string> Glenum_to_str {
		  { GL_VERTEX_SHADER	, "GL_VERTEX_SHADER"	}
		, { GL_FRAGMENT_SHADER	, "GL_FRAGMENT_SHADER"	}
	};


	char const * open_gl_version = "#version 330 core";

	char const * vertex_shader_src =
		"#version 330 core\n"
		"layout(location=0) in vec3 vertex_position;\n"
		"void main(){\n"
		"gl_Position.xyz = vertex_position;"
		"gl_Position.w = 1.0; }";

	// fragment shaders process pixel color
	char const * fragment_shader_src =
		"#version 330 core\n"
		"out vec3 color;\n"
		"void main(){\n"
		"color = vec3(1.0, 0.0, 0.0); }";

	GLfloat const verticies[9] = {
		//  x	   y	z
		 -0.5f, -0.5f, 0.0f		// 0
		, 0.5f, -0.5f, 0.0f		// 1
		, 0.0f,  0.5f, 0.0f		// 2
	};

	bool draw_imgui_main_menu_bar = true;
	bool draw_imgui_debug_window = false;

	void paint_imgui_main_menu_bar();
	void paint_imgui_console_window();

protected:

	static std::unique_ptr<RenderManager> SDL2_render_manager;

	std::unique_ptr<OpenGlShaderProgram> basic_shader;
	GLuint active_shader_program_id = NULL;

	SDL_Window * program_window		= nullptr;

	SDL_GLContext opengl_context	= nullptr;

	SDL_Renderer * active_renderer	= nullptr;

	SDL_Surface * rendering_surface = nullptr;

	SceneManager2D * scene_manager	= nullptr;

	Console * debug_console			= nullptr;

	SDL_RendererInfo renderer_info;

	RenderObjectsVector render_objects;


	// screen clear color
	ImVec4 scc;  //{0.2f, 0.3f, 0.3f, 1.0f}

	unsigned int screen_width = 0;
	unsigned int screen_height = 0;

	// stores info on state of vertex data, (& format)
	GLuint vertex_array_id;

	// stores the vertex data
	GLuint vertex_buffer_id;

	bool init_sdl(unsigned xOffset, unsigned yOffset, unsigned Width, unsigned Height, char const * WindowTitle);	
	bool init_opengl();
	bool init_sdl_image();

	bool init_shaders();
	bool init_geometry();

	bool init_imgui();

public:
	RenderManager()
		: renderer_state(ERendererState::CONSTRUCTED)	
	{}
	

	bool init_system(
		  unsigned xOffset = SDL_WINDOWPOS_UNDEFINED
		, unsigned yOffset = SDL_WINDOWPOS_UNDEFINED
		, unsigned Width   = 0
		, unsigned Height  = 0
		, bool fullScreen	   = false
		, char const * WindowTitle = 0
	);

	bool update();

	void shutdown();

	SDL_Renderer * get_active_renderer() { return active_renderer; }

	void render_visible_scene_back_to_front();

	void set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect);
			
	qSceneObject * register_render_object(qRenderResource * non_owner, bool is_visible = true);

	RenderObject2D * get_render_object(int id);
	
	void set_scene_manager(SceneManager2D * sm) { scene_manager = sm; }

	ERendererState const get_renderer_state() const { return renderer_state; }

	void toggle_imgui_debug_window() { draw_imgui_debug_window = !draw_imgui_debug_window; }
	
	void set_debug_console(Console * con) { debug_console = con; }
};


namespace witchcraft
{
	namespace rendering
	{
		int constexpr OPENGL_MAJOR_VERSION = 3;
		int constexpr OPENGL_MINOR_VERSION = 3;

		SDL_GLprofile constexpr OPENGL_PROFILE = SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE;
	}
}


#endif // RENDER_MANAGER_H
