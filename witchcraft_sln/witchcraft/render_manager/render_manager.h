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


// witchcraft
#include "../engine/engine_object.h"

#include "../message_bus/message_bus.h"

#include "../resource_manager/resource.h"
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

	std::map<GLenum, std::string> Glenum_to_str{
		  { GL_VERTEX_SHADER	, "GL_VERTEX_SHADER"	}
		, { GL_FRAGMENT_SHADER	, "GL_FRAGMENT_SHADER"	}
	};


	char const * open_gl_version = "#version 330 core";

	char const * vertex_shader_src =
		"#version 330 core\n"
		"layout(location=0) in vec3 position;\n"
		"void main(){\n"
		"gl_Position.xyz = position;"
		"gl_Position.w = 1.0f; }";

	// fragment shaders process pixel color
	char const * fragment_shader_src =
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main(){\n"
		"color = vec4(1.0f, 0.0f, 0.0f, 1.0f); }";

	char const * sprite_vertex_shader_src =
		"#version 330 core/n"
		"layout(location=0) in vec4 vertex;" // <vec2 pos, vec2 textureCoordinate>
		"out vec2 texture_coordinates;"
		"uniform mat4 model;"
		"uniform mat4 projection;"
		"void main(){"
		"texture_coordinates = vertex.zw;"
		"gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);}";

	char const * sprite_fragment_shader_src =
		"#version 330 core\n"
		"in vec2 texture_coordinates;"
		"out vec3 color;"
		"uniform sampler2D image;"
		"uniform vec3 sprite_color;"
		"void main(){"
		"color = vec4(sprite_color, 1.0) * texture(image, texture_coordinates);}";


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

	std::unique_ptr<OpenGlShaderProgram> basic_shader;
	std::unique_ptr<OpenGlShaderProgram> sprite_shader;
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

	//void draw_sprite(qSceneObject const & so)
	//{
	//	sprite_shader->SetActive();
	//
	//	glm::mat4 model_transform = glm::mat4(1.0f);
	//	
	//	// translate
	//	auto pos = so.get_position();
	//	model_transform = glm::translate(
	//		model_transform
	//		, glm::vec3(
	//			  std::get<0>(pos)
	//			, std::get<1>(pos), 1.0f
	//		)
	//	);
	//
	//
	//	auto wh = so.render_resource->get_width_height();
	//	// move model, such that it's pivot point is in the middle
	//	model_transform = glm::translate(
	//		model_transform
	//		, glm::vec3(
	//			  0.5f * std::get<0>(wh)
	//			, 0.5f * std::get<1>(wh)
	//			, 0.0f)
	//	);
	//
	//	// rotate
	//	model_transform = glm::rotate(
	//		model_transform
	//		, glm::radians(so.get_rotation())
	//		, glm::vec3(0.0f, 0.0f, 1.0f)
	//	);
	//
	//	// move model, such that it's pivot point is in the corner again
	//	model_transform = glm::translate(
	//		model_transform
	//		, glm::vec3(
	//			  -0.5f * std::get<0>(wh)
	//			, -0.5f * std::get<1>(wh)
	//			,  0.0f)
	//	);
	//
	//
	//
	//	// scale
	//	auto scale = so.get_scale();
	//	model_transform = glm::translate(
	//		model_transform
	//		, glm::vec3(
	//			  std::get<0>(scale)
	//			, std::get<1>(scale)
	//			, 0.0f
	//		)
	//	);
	//
	//	//sprite_shader->SetMatrix4("model", model_transform);
	//	//sprite_shader->SetVector3f("sprite_color", color);
	//
	//	glActiveTexture(GL_TEXTURE0);
	//	//glBindTexture(GL_TEXTURE_2D, so.render_resource->texture);
	//
	//	glBindVertexArray(this->sprite_quad_ebo);
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//	glBindArray(0);
	//}

public:
	RenderManager()
		: renderer_state(ERendererState::CONSTRUCTED)	
	{}

	RenderManager(MessageBus * mb)
		: renderer_state(ERendererState::CONSTRUCTED)
		, message_bus(mb)
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
	}
}


#endif // RENDER_MANAGER_H
