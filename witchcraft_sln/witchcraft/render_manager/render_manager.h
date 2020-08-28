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

// witchcraft
#include "../engine/engine_object.h"

#include "../resource_manager/resource.h"
#include "../string_constants.h"
#include "../utility/utility.h"
#include "render_object_2d.h"
#include "render_resource.h"

#include "../scene_manager/scene_manager_2d.h"
#include "../scene_manager/scene_object.h"

class SceneManager2D;
class qSceneObject;

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



	char const * vertex_shader_src =
		"#version 330 core\n"
		"layout(location=0) in vec2 posAttr;\n"
		"void main(){\n"
		"gl_Position = vec4(posAttr, 0.0, 1.0); }";

	// fragment shaders process pixel color
	char const * fragment_shader_src =
		"#version 330 core\n"
		"out vec4 col;\n"
		"void main(){\n"
		"col = vec4(1.0, 0.0, 0.0, 1.0); }";

	GLfloat const verticies[9] = {
		 -0.5f, -0.5f, 0.0f
		, 0.5f, -0.5f, 0.0f
		, 0.0f,  0.5f, 0.0f
	};

	//GLfloat const verts2[4][4] = {
	//	  { -1.0f, -1.0f, 0.0f, 1.0f }
	//	, { -1.0f,  1.0f, 0.0f, 0.0f }
	//	, {  1.0f,  1.0f, 1.0f, 0.0f }
	//	, {  1.0f, -1.0f, 1.0f, 1.0f }
	//};
	//
	//GLfloat const idxes2[6] = {
	//	0, 1, 2, 0, 2, 3
	//};

protected:
	static std::unique_ptr<RenderManager> SDL2_render_manager;

	SDL_Window * program_window		= nullptr;

	SDL_GLContext opengl_context	= nullptr;

	SDL_Renderer * active_renderer	= nullptr;

	SDL_Surface * rendering_surface = nullptr;

	SceneManager2D * scene_manager	= nullptr;

	SDL_RendererInfo renderer_info;

	RenderObjectsVector render_objects;
	
	unsigned int screen_width = 0;
	unsigned int screen_height = 0;

	// stores info on state of vertex data, (& format)
	GLuint vertex_array_id;

	// stores the vertex data
	GLuint vertex_buffer_id;

	// handles for shaders
	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint shader_program_id;

	bool init_sdl(unsigned xOffset, unsigned yOffset, unsigned Width, unsigned Height, char const * WindowTitle);	
	bool init_opengl();
	bool init_sdl_image();

	bool init_shaders();
	bool init_geometry();

	bool compile_shader(GLuint id, GLenum type, char const * src);
	bool link_shader_program(GLuint vertx_id, GLuint frag_id, GLuint program_id);

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
