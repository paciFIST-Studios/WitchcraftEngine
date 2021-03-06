#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <glm/glm.hpp>

#include "../render_manager/opengl_texture.h"
#include "../engine/engine_object.h"

// each instance represents a unique renderable item
// It contains position, scale, & rotation, and these
// characteristics will allow the render manager to perform culling.
class RenderObject : public EngineObjectBase
{
private:
protected:
	glm::f32vec3 position;
	glm::f32vec3 rotation;
	glm::f32vec3 scale;

	bool visible;

	GLuint vertex_array_object;
	unsigned vertex_elements;

	OpenGLTexture * texture;

public:
	 RenderObject();
	~RenderObject();

	RenderObject(
		  OpenGLTexture * tex
		, glm::f32vec3  * pos = nullptr
		, glm::f32vec3  * roa = nullptr
		, glm::f32vec3  * scl = nullptr
	)
		: texture(tex)
	{
		position = (pos) ? *pos : glm::f32vec3(0.0f,0.0f,0.0f);
		rotation = (roa) ? *roa : glm::f32vec3(0.0f,0.0f,0.0f);
		scale    = (scl) ? *scl : glm::f32vec3(0.0f,0.0f,0.0f);
	}

	OpenGLTexture * get_texture() const;
	void set_texture(OpenGLTexture & texture);

	glm::f32vec3 get_position() const;
	void set_position(glm::f32vec3 const & pos);

	glm::f32vec3 get_rotation() const;
	void set_rotation(glm::f32vec3 const & rotation);

	glm::f32vec3 get_scale() const;
	void set_scale(glm::f32vec3 const & scale);
	
	bool is_visible() const;
	void set_is_visible(bool is);
	   

	// - for rendering -----------------------------------------------------------------

	void set_initialized_vao(unsigned const vao, unsigned const elements)
	{
		vertex_array_object = vao;
		vertex_elements = elements;
	}
	
	void draw()
	{
		if (texture)
		{
			texture->bind();
			glBindVertexArray(vertex_array_object);
			glDrawElements(GL_TRIANGLES, vertex_elements, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
};

#endif // !RENDER_OBJECT_H
