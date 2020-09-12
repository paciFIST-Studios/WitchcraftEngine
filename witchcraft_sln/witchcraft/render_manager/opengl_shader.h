#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

//https://learnopengl.com/code_viewer_gh.php?code=src/7.in_practice/3.2d_game/0.full_source/shader.h

#include <string>

#include <SDL.h>
#include <GL/glew.h>

#include "../engine/engine_object.h"

class OpenGlShaderProgram : public qEngineObject
{
private:
	// not copyable
	OpenGlShaderProgram(OpenGlShaderProgram const &) = delete;
	// not assignable
	OpenGlShaderProgram& operator=(OpenGlShaderProgram const &) = delete;

protected:
	unsigned int shader_program_id = 0;

	bool check_compile_errors(unsigned int id, char const * type);

	// constants used for messaging state inside the class
	char const * VERTEX		= "VERTEX";
	char const * FRAGMENT	= "FRAGMENT";
	char const * GEOMETRY	= "GEOMETRY";
	char const * PROGRAM	= "PROGRAM";

	GLuint compile_vertex_shader(char const * src);
	GLuint compile_fragment_shader(char const * src);
	GLuint compile_geometry_shader(char const * src);
	GLuint link_program(GLuint vert, GLuint frag, GLuint geo = NULL);

public:

	OpenGlShaderProgram(){}
	~OpenGlShaderProgram() { if(this->shader_program_id > 0)	glDeleteProgram(this->shader_program_id); }



	unsigned int get_shader_program_id() { return shader_program_id; }

	OpenGlShaderProgram & SetActive();

	void compile(char const * vertex_src, char const * fragment_src, char const * geometry_src = nullptr);

	void set_float(char const * name, float value, bool use_shader = false);
	void set_int(char const * name, int value, bool use_shader = false);
	void set_vec2f(char const * name, float x, float y, bool use_shader = false);

};


#endif // !OPENGL_SHADER_H
