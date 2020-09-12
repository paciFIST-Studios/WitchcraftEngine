#include "opengl_shader.h"

OpenGlShaderProgram & OpenGlShaderProgram::SetActive()
{
	glUseProgram(this->shader_program_id);
	return *this;
}

GLuint OpenGlShaderProgram::compile_vertex_shader(char const * src)
{
	if (src == nullptr || src == "") { return NULL; }

	GLuint vert;
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &src, NULL);
	glCompileShader(vert);
	check_compile_errors(vert, VERTEX);
	return vert;
}

GLuint OpenGlShaderProgram::compile_fragment_shader(char const * src)
{
	if (src == nullptr || src == "") { return NULL; }
	
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &src, NULL);
	glCompileShader(frag);
	check_compile_errors(frag, FRAGMENT);
	return frag;
}

GLuint OpenGlShaderProgram::compile_geometry_shader(char const * src)
{
	if (src == nullptr || src == "") { return NULL; }

	GLuint geo = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geo, 1, &src, NULL);
	glCompileShader(geo);
	check_compile_errors(geo, GEOMETRY);
	return geo;
}

GLuint OpenGlShaderProgram::link_program(GLuint vert, GLuint frag, GLuint geo)
{
	this->shader_program_id = glCreateProgram();
	glAttachShader(this->shader_program_id, vert);
	glAttachShader(this->shader_program_id, frag);
	if (geo != NULL)
	{
		glAttachShader(this->shader_program_id, geo);
	}

	glLinkProgram(this->shader_program_id);
	check_compile_errors(this->shader_program_id, PROGRAM);

	glDetachShader(this->shader_program_id, vert);
	glDetachShader(this->shader_program_id, frag);
	if (geo != NULL)
	{
		glDetachShader(this->shader_program_id, vert);
	}

	return this->shader_program_id;
}


void OpenGlShaderProgram::compile(char const * vertex_src, char const * fragment_src, char const * geometry_src)
{
	GLuint vert, frag, geo;

	vert = compile_vertex_shader(vertex_src);
	frag = compile_fragment_shader(fragment_src);
	geo  = compile_geometry_shader(geometry_src);

	link_program(vert, frag, geo);	

	glDeleteShader(vert);
	glDeleteShader(frag);
	glDeleteShader(geo);
}


bool OpenGlShaderProgram::check_compile_errors(unsigned int obj, char const * type)
{
	int success;
	std::unique_ptr<char[]> info_log(new char[1024]);

	// NOTE: don't compare success to true or GL_TRUE, as per
	// https://gamedev.stackexchange.com/a/156583

	if (type == VERTEX || type == FRAGMENT || type == GEOMETRY)
	{
		glGetShaderiv(obj, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(obj, 1024, NULL, info_log.get());
			PLOGF << "FAIL: shader compile: [" << type << "]\n\t" << info_log.get();
			return false;
		}
		else
		{
			PLOGV << "SUCCESS: shader compile: [" << type << "]";
		}
	}
	// else if (compute shader)
	else if (type == PROGRAM)
	{
		glGetProgramiv(obj, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(obj, 1024, NULL, info_log.get());
			PLOGF << "FAIL: shader program link:\n\t" << info_log.get();
			return false;
		}
		else
		{
			PLOGV << "SUCCESS: shader program link";
		}
	}
	else
	{
		PLOGF << "FAIL: UNKNOWN ERROR: User Supplied Shader Type = [" << type << "]";
		return false;
	}

	// returns true if no compile error
	return true;
}
