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
#include <SDL_surface.h>
#include <SDL_video.h>

// opengl
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// rapidxml
#include "../../lib/rapidxml/rapidxml.hpp"
#include "../../lib/rapidxml/rapidxml_utils.hpp"
#define XML rapidxml

// imgui
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl.h"
#include "../imgui/imgui_impl_opengl3.h"

// STB_Image
#include "../stb_image.h"

// witchcraft
#include "../message_bus/message_bus.h"

#include "../resource_manager/engine_resource_base.h"
#include "../string_constants.h"
#include "../utility/utility.h"
#include "render_object.h"
#include "sdl_render_resource.h"

#include "../scene_manager/scene_manager_2d.h"
#include "../scene_manager/scene_object.h"

#include "opengl_shader.h"
#include "opengl_texture.h"
#include "../render_manager/shader_resource.h"
#include "../resource_manager/vertex_resource.h"

#include "../console/console.h"


class SceneManager2D;

enum class ERendererState : unsigned char
{
	  UNINITIALIZED	= 0x01
	, CONSTRUCTED	= 0x02
	, SDL_INIT_OK	= 0x04
	, OPENGL_INIT_OK= 0x08
	, UPDATE_OK		= 0x10
	, SHUTDOWN_START= 0x20
	, SDL_QUIT_OK	= 0x40
	, SHUTDOWN_OK	= 0x80
};

struct WindowStats
{
	int x;
	int y;
	int w;
	int h;
	Uint32 flags;
};

struct OpenGLSpriteQuad
{
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};

class RenderManager : public EngineObjectBase
{
public:
	

private:
	ERendererState renderer_state = ERendererState::UNINITIALIZED;

	char const * open_gl_version = "#version 330 core";

	char const * basic_vertex_src =
		"#version 330 core\n"
		"layout(location=0) in vec3 aPos;\n"
		"layout(location=1) in vec3 aColor;\n"
		"layout(location=2) in vec2 aUV;\n"
		"out vec3 _color;\n"
		"out vec2 _uv;\n"
		"void main(){\n"
		"gl_Position = vec4(aPos, 1.0f);\n"
		"_color = aColor;\n"
		"_uv = aUV;\n"
		"}"
		;

	char const * basic_fragment_src =
		"#version 330 core\n"
		"out vec4 fColor;\n"
		"in vec3 _color;\n"
		"in vec2 _uv;\n"
		"uniform sampler2D _texture;\n"
		"void main(){\n"
		"fColor = texture(_texture, _uv);\n"
		"}"
		;
	
	const char * textureless_vertex_src = 
		"#version 330 core\n"
		"layout(location=0) in vec3 pos;\n"
		"void main() {\n"
		"gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);\n}"
		;

	char const * textureless_fragment_src =
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main(){\n"
		"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n}"
		;
	

	glm::mat4 model_matrix;
	glm::mat4 view_matrix;
	glm::mat4 orthographic_projection_matrix;
	glm::mat4 perspective_projection_matrix;

	bool use_vertex_class_not_quad = false;
	bool use_wireframe_rendering = false;
	bool draw_triangle_not_quad = false;
	bool draw_imgui_main_menu_bar = true;
	bool draw_imgui_debug_window = false;


	void paint_imgui_main_menu_bar();

	void paint_debug_windows();

	void initialize_sprite_quad(VertexResource const * vert, OpenGLSpriteQuad & quad);

protected:

	static std::unique_ptr<RenderManager> render_manager;
	std::vector<std::unique_ptr<RenderObject>> render_objects;

	unsigned int active_shader_idx = 0;
	std::vector<std::unique_ptr<OpenGlShaderProgram>> shader;

	char const * active_shader = "";
	std::map<char const *, std::unique_ptr<OpenGlShaderProgram>> shaders;

	std::map<char const *, std::unique_ptr<OpenGLTexture>> textures;

	bool use_texture_class = false;
	OpenGLTexture sprite_texture = OpenGLTexture("buddha_texture", "asset/buddha.png");
	OpenGLSpriteQuad sprite_quad;


	SDL_Window * program_window		= nullptr;

	SDL_GLContext opengl_context	= nullptr;

	SDL_Renderer * active_renderer	= nullptr;

	SDL_Surface * rendering_surface = nullptr;

	SceneManager2D * scene_manager	= nullptr;

	Console * debug_console			= nullptr;

	MessageBus * message_bus		= nullptr;


	SDL_RendererInfo renderer_info;

	// screen clear color
	ImVec4 scc;  //{0.2f, 0.3f, 0.3f, 1.0f}

	unsigned int screen_width = 0;
	unsigned int screen_height = 0;

	bool init_sdl_window(unsigned xOffset, unsigned yOffset, unsigned Width, unsigned Height, char const * WindowTitle);	
	bool init_opengl();

	bool init_shaders();
	bool init_geometry();

	bool init_imgui();

	void request_debug_console_ptr();


	inline bool contains_term(std::string const * source, char const * search)
	{
		return source->find(search) != std::string::npos;
	}
	
	void handle_message(Message m);

	void handle_supply_console_ptr_message(Message & m);
	void handle_invoke_render_command(Message & m);
	void handle_supply_resource(Message & m);

	unsigned int engine_channel_id   = 0;
	unsigned int render_channel_id   = 0;
	unsigned int resource_channel_id = 0;


public:
	RenderManager()
		: renderer_state(ERendererState::CONSTRUCTED)	
	{}

	RenderManager(MessageBus * mb)
		: renderer_state(ERendererState::CONSTRUCTED)
		, message_bus(mb)
	{
		std::function<void(Message)> cb = std::bind(&RenderManager::handle_message, this, std::placeholders::_1);
		message_bus->subscribe("render", cb);

		// cache these locally, b/c we're likely to use them very often
		render_channel_id = message_bus->channel_lookup("render");
		engine_channel_id = message_bus->channel_lookup("engine");
		resource_channel_id = message_bus->channel_lookup("resource");
	}

	void init_system(
		  unsigned xOffset = SDL_WINDOWPOS_UNDEFINED
		, unsigned yOffset = SDL_WINDOWPOS_UNDEFINED
		, unsigned Width   = 0
		, unsigned Height  = 0
		, bool fullScreen  = false
		, char const * WindowTitle = 0
	);

	bool update();

	void shutdown();

	WindowStats get_window_stats() const 
	{ 
		WindowStats stats;
		SDL_GetWindowSize(program_window, &stats.w, &stats.h);
		SDL_GetWindowPosition(program_window, &stats.x, &stats.y);
		stats.flags = SDL_GetWindowFlags(program_window);
		return stats;
	}

	SDL_Renderer * get_active_renderer() { return active_renderer; }

	void render_visible_sprites_back_to_front();

	void render_visible_scene_back_to_front();

	void set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect);
			
	qSceneObject * register_render_object(SDLRenderResource * non_owner, bool is_visible = true);

	RenderObject * get_render_object(int id);
	
	void set_scene_manager(SceneManager2D * sm) { scene_manager = sm; }

	ERendererState const get_renderer_state() const { return renderer_state; }

	void toggle_imgui_debug_window() { draw_imgui_debug_window = !draw_imgui_debug_window; }
	
	void set_debug_console(Console * con) { debug_console = con; }

	void toggle_wireframe_rendering() { use_wireframe_rendering = !use_wireframe_rendering; }

	void switch_triangle_and_quad() { draw_triangle_not_quad = !draw_triangle_not_quad; }
};


namespace witchcraft
{
	namespace rendering
	{
		int constexpr OPENGL_MAJOR_VERSION = 3;
		int constexpr OPENGL_MINOR_VERSION = 3;

		SDL_GLprofile constexpr OPENGL_PROFILE = SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE;

	}  // ! namespace rendering
}  // ! namespace witchcraft


#endif // RENDER_MANAGER_H
