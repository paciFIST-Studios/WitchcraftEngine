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

// opengl
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// image loader lib
//#define STB_IMAGE_IMPLEMENTATION
//#define STBI_FAILURE_USERMSG
//#include "../stb_image.h"


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

#include "../message_bus/message_bus.h"

#include "../resource_manager/engine_resource_base.h"
#include "../string_constants.h"
#include "../utility/utility.h"
#include "render_object_2d.h"
#include "sdl_render_resource.h"

#include "../scene_manager/scene_manager_2d.h"
#include "../scene_manager/scene_object.h"

#include "opengl_shader.h"

#include "../console/console.h"


class SceneManager2D;
class qSceneObject;
class Console;

enum class ERendererState : unsigned char
{
	  UNINITIALIZED		= 0x01
	, CONSTRUCTED	= 0x02
	, SDL_INIT_OK	= 0x04
	, OPENGL_INIT_OK= 0x08
	, UPDATE_OK		= 0x10
	, SHUTDOWN_START= 0x20
	, SDL_QUIT_OK	= 0x40
	, SHUTDOWN_OK	= 0x80
};

struct OpenGLTexture
{
	int width;
	int height;
	int color_channels;
	unsigned int texture_id;
};

class RenderManager : public EngineObjectBase
{
public:
	typedef std::vector<std::unique_ptr<RenderObject2D>> RenderObjectsVector;

private:
	ERendererState renderer_state = ERendererState::UNINITIALIZED;

	std::map<GLenum, std::string> Glenum_to_str{
		  { GL_VERTEX_SHADER	, "GL_VERTEX_SHADER"	}
		, { GL_FRAGMENT_SHADER	, "GL_FRAGMENT_SHADER"	}
	};

	char const * open_gl_version = "#version 330 core";


	char const * sprite_vertex_shader_src =
		"#version 330 core/n"
		"layout(location=0) in vec2 vertex;\n"
		"layout(location=1) in vec2 _uv;\n" // texture coordinate
		"layout(location=2) in vec2 _wh;\n" // screen width and height
		"out vec2 uv;\n"
		"void main(){\n"
		"uv = _uv;\n"
		"gl_Position = vec4(vertex.x / wh.x -1.0, vertex.y / wh.h - 1.0, 0.0, 1.0);}\n";

	char const * sprite_fragment_shader_src =
		"#version 330 core\n"
		"out vec4 color;\n"
		"in vec2 uv;\n"
		"uniform sampler2D tex;"
		"void main(){"
		"color = texture(tex, uv);\n}\n";


	GLfloat const triangle_verticies[9] = {
		//  x	   y	z
		 -0.5f, -0.5f, 0.0f		// 0
		, 0.5f, -0.5f, 0.0f		// 1
		, 0.0f,  0.5f, 0.0f		// 2
	};

	// this is two triangles
	//GLfloat const sprite_verticies[24] = {
	//	// pos			// texture
	//	  0.0f, 1.0f	, 0.0f, 1.0f
	//	, 1.0f, 0.0f	, 1.0f, 0.0f
	//	, 0.0f, 0.0f	, 0.0f, 0.0f
	//
	//	, 0.0f, 1.0f	, 0.0f, 1.0f
	//	, 1.0f, 1.0f	, 1.0f, 1.0f
	//	, 1.0f, 0.0f	, 1.0f, 0.0f
	//};

	GLfloat const sprite_verticies[12] = {
		   0.5f,  0.5f, 0.0f // top right
		,  0.5f, -0.5f, 0.0f // bottom right
		, -0.5f, -0.5f, 0.0f // bottom left
		, -0.5f,  0.5f, 0.0f // top left
	};
	GLuint const sprite_indicies[6] = {
		  0, 1, 3 // t1
		, 1, 2, 3 // t2
	};

	GLuint sprite_quad_vao;	// vertex array object
	GLuint sprite_quad_vbo;	// vertex buffer object
	GLuint sprite_quad_ebo; // element buffer object

	glm::mat4 projection_matrix;

	bool use_wireframe_rendering = false;
	bool draw_triangle_not_quad = false;

	bool draw_imgui_main_menu_bar = true;
	bool draw_imgui_debug_window = false;

	void paint_imgui_main_menu_bar();

protected:

	static std::unique_ptr<RenderManager> SDL2_render_manager;

	std::map<char const *, std::unique_ptr<OpenGlShaderProgram>> shaders;

	GLuint active_shader_program_id = NULL;

	SDL_Window * program_window		= nullptr;

	SDL_GLContext opengl_context	= nullptr;

	SDL_Renderer * active_renderer	= nullptr;

	SDL_Surface * rendering_surface = nullptr;

	SceneManager2D * scene_manager	= nullptr;

	Console * debug_console			= nullptr;

	MessageBus * message_bus		= nullptr;

	SDL_RendererInfo renderer_info;

	RenderObjectsVector render_objects;

	// screen clear color
	ImVec4 scc;  //{0.2f, 0.3f, 0.3f, 1.0f}

	unsigned int screen_width = 0;
	unsigned int screen_height = 0;

	// stores info on state of vertex data, (& format)
	GLuint triangle_vao;

	// stores the vertex data
	GLuint triangle_vbo;

	bool init_sdl(unsigned xOffset, unsigned yOffset, unsigned Width, unsigned Height, char const * WindowTitle);	
	bool init_opengl();
	bool init_sdl_image();

	bool init_shaders();
	bool init_geometry();

	bool init_imgui();

	void init_get_debug_console();


	inline bool contains_term(std::string const * source, char const * search)
	{
		return source->find(search) != std::string::npos;
	}


	void handle_message(Message m);

	unsigned int engine_channel_id = 0;
	unsigned int render_channel_id = 0;



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
	}

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

	void render_visible_sprites_back_to_front();

	void render_visible_scene_back_to_front();

	void set_surface_RGB(unsigned int r, unsigned int g, unsigned int b, SDL_Rect const * rect);
			
	qSceneObject * register_render_object(SDLRenderResource * non_owner, bool is_visible = true);

	RenderObject2D * get_render_object(int id);
	
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

	

		//static OpenGLTexture get_opengl_texture(char const & filepath)
		//{
		//	struct OpenGLTexture result { -1, -1, -1, 0 };
		//
		//	if (!utility::file_exists(&filepath)) {
		//		return result;
		//	}
		//
		//	//unsigned char * data = stbi_load(
		//	//	&filepath
		//	//	, &result.width
		//	//	, &result.height
		//	//	, &result.color_channels
		//	//	, 0
		//	//);
		//	//
		//	//if (data != nullptr)
		//	//{
		//	//	glGenTextures(1, &result.texture_id);
		//	//	glBindTexture(GL_TEXTURE_2D, result.texture_id);
		//	//
		//	//	// todo: set wrapping options in fn call
		//	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	//
		//	//	glTexImage2D(
		//	//		GL_TEXTURE_2D		// 1d and 3d textures also exist
		//	//		, 0					// mipmap level (default=0)
		//	//		, GL_RGB			// storage format
		//	//		, result.width		// image width
		//	//		, result.height		// image height
		//	//		, 0					// legacy-unused
		//	//		, GL_RGB			// source format
		//	//		, GL_UNSIGNED_BYTE  // source format
		//	//		, data				//
		//	//	);
		//	//	glGenerateMipmap(GL_TEXTURE_2D);
		//	//
		//	//	PLOGV << "Texture Loaded: " << filepath
		//	//		<< "\n\t width: " << result.width
		//	//		<< "\t height: " << result.height
		//	//		<< "\n\t size: " << sizeof(data)
		//	//		;
		//	//
		//	//	stbi_image_free(data);
		//	//}
		//	//else
		//	//{
		//	//	// record as error (ploge), not as fatal (plogf)
		//	//	PLOGE << "FAILURE!: Texture not loaded"
		//	//		<< "\n\tfilepath: " << filepath
		//	//		<< "\n\terror: " << stbi_failure_reason()
		//	//		;
		//	//}
		//
		//	return result;
		//}

	}  // ! namespace rendering
}  // ! namespace witchcraft


#endif // RENDER_MANAGER_H
