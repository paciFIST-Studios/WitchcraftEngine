#ifndef OPENGL_SHADER__TST_CC
#define OPENGL_SHADER__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../render_manager/opengl_shader.h"
		
		class test_OpenGLShaderProgram : public OpenGlShaderProgram
		{
		public:
			char const * passing_vert_src = "#version 330 core\n"
				"layout ( location = 0 ) in vec3 pos;\n"
				"void main(){\n"
				"gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);}";
			char const * failing_vert_src = "";

			char const * passing_frag_src = "#version 330 core\n"
				"out vec4 color;\n"
				"void main(void){\n"
				"color = vec4(1.0f, 1.0f, 1.0f, 1.0f)}\n";
			char const * failing_frag_src = "";

			char const * passing_geo_src = "";
			char const * failing_geo_src = "";

			GLuint test_compile_vertex_shader(char const * src)
			{
				return compile_vertex_shader(src);
			}

			GLuint test_compile_fragment_shader(char const * src)
			{
				return compile_fragment_shader(src);
			}

			GLuint test_compile_geometry_shader(char const * src)
			{
				return compile_geometry_shader(src);
			}

			bool test_link_program(GLuint vert, GLuint frag, GLuint geo = NULL)
			{
				return link_program(vert, frag, geo);
			}

			bool test_no_compile_error(unsigned int obj, char const * type) 
			{ 
				return check_compile_errors(obj, type); 
			}

		};

		TEST_CASE("OpenGLShaderProgram:: ctor  constructs object w/ default params")
		{
			auto shader = &OpenGlShaderProgram();
			REQUIRE(shader);
		}
	
		//TEST_CASE("OpenGLShaderProgram:: compiles  passing vert src without error")
		//{
		//	auto shader = test_OpenGLShaderProgram();
		//	auto src = shader.passing_vert_src;
		//	auto pass = shader.test_compile_vertex_shader(src);
		//	REQUIRE(pass);
		//}

		//
		//TEST_CASE("OpenGLShaderProgram:: shows  compile error for failing vert src")
		//{
		//	auto s = test_OpenGLShaderProgram();
		//	auto pass = s.test_compile_fragment_shader(s.failing_frag_src);
		//	REQUIRE_FALSE(pass);
		//}
		//
		//TEST_CASE("OpenGLShaderProgram:: compiles  passing frag src without error"){}
		//TEST_CASE("OpenGLShaderProgram:: shows  compile error for failing frag src"){}


	#endif // RUN_UNIT_TESTS

#endif // !OPENGL_SHADER__TST_CC


