#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../engine/engine_object.h"

class OpenGlShaderProgram : public EngineObjectBase
{
private:
	//// not copyable
	//OpenGlShaderProgram(OpenGlShaderProgram const &) = delete;
	//// not assignable
	//OpenGlShaderProgram& operator=(OpenGlShaderProgram const &) = delete;

protected:
	// constants used for messaging state inside the class
	char const * VERTEX		= "VERTEX";
	char const * FRAGMENT	= "FRAGMENT";
	char const * GEOMETRY	= "GEOMETRY";
	char const * COMPUTE	= "COMPUTE";
	char const * PROGRAM	= "PROGRAM";
	
	unsigned int shader_program_id = 0;

	bool check_compile_errors(unsigned int id, char const * type);

	GLuint compile_vertex_shader(char const * src);
	GLuint compile_fragment_shader(char const * src);
	GLuint compile_geometry_shader(char const * src);
	GLuint link_program(GLuint vert, GLuint frag, GLuint geo = NULL);

public:
	OpenGlShaderProgram(){}
	~OpenGlShaderProgram() { if(this->shader_program_id != 0) glDeleteProgram(this->shader_program_id); }

	unsigned int get_shader_program_id() { return shader_program_id; }

	void use_program();

	void compile(char const * vertex_src, char const * fragment_src, char const * geometry_src = nullptr);

	void setInt( char const * name, int value) const;
	void setVec2(char const * name, glm::vec2 const & value) const;
	void setVec3(char const * name, glm::vec3 const & value) const;
	void setVec4(char const * name, glm::vec4 const & value) const;

	void setMat4(char const * name, glm::mat4 const & value) const;
};

#endif // !OPENGL_SHADER_H
