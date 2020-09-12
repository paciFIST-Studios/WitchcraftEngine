#ifndef OPENGL_SHADER__TST_CC
#define OPENGL_SHADER__TST_CC

// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../render_manager/opengl_shader.h"
		
		TEST_CASE("OpenGLShaderProgram:: ctor  constructs object w/ default params")
		{
			auto shader = &OpenGlShaderProgram();
			REQUIRE(shader);
		}
	
	#endif // RUN_UNIT_TESTS

#endif // !OPENGL_SHADER__TST_CC


