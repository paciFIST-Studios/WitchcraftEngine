#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

//https://learnopengl.com/code_viewer_gh.php?code=src/7.in_practice/3.2d_game/0.full_source/shader.h

#include <string>

#include "../engine/engine_object.h"

class OpenGlShader : public qEngineObject
{
private:
	unsigned int shader_id;

	void check_compile_error(unsigned int obj, char const * type);

protected:
public:
	OpenGlShader() {}

	unsigned int shader_id() { return shader_id; }

	OpenGlShader &Use();

	void compile(char const * vertex_src, char const * fragment_src, char const * geometry_src = nullptr);

	void set_float(char const * name, float value, bool use_shader = false);
	void set_int(char const * name, int value, bool use_shader = false);
	void set_vec2f(char const * name, float x, float y, bool use_shader = false);

};


#endif // !OPENGL_SHADER_H
