#ifndef RENDER_OBJECT_TWO_D__TST_CC
#define RENDER_OBJECT_TWO_D__TST_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"

		#include "../uninitialized_values.h"
		#include "render_object.h"

		TEST_CASE(" RenderObject::ctor  default ctor initializes RenderObject to default values"  )
		{
			auto ro = RenderObject();
			REQUIRE_NOTHROW(ro.get_position());
			REQUIRE_NOTHROW(ro.get_rotation());
			REQUIRE_NOTHROW(ro.get_scale()   );

			auto pos = ro.get_position();
			auto roa = ro.get_rotation();
			auto scl = ro.get_scale();
		
			REQUIRE(typeid(pos) == typeid(glm::f32vec3));
			REQUIRE(typeid(roa) == typeid(glm::f32vec3));
			REQUIRE(typeid(scl) == typeid(glm::f32vec3));
			
			REQUIRE(pos.x == 0.0f);
			REQUIRE(pos.y == 0.0f);
			REQUIRE(pos.z == 0.0f);

			REQUIRE(roa.x == 0.0f);
			REQUIRE(roa.y == 0.0f);
			REQUIRE(roa.z == 0.0f);

			REQUIRE(scl.x == 0.0f);
			REQUIRE(scl.y == 0.0f);
			REQUIRE(scl.z == 0.0f);

			REQUIRE(ro.is_visible() == false);
			REQUIRE_FALSE(ro.is_visible());

			auto texture = ro.get_texture();
			REQUIRE(texture == nullptr);
		}

		TEST_CASE(" RenderObject:: ctor  initializes position, rotation, and scale  correctly")
		{
			glm::f32vec3 p = glm::f32vec3(1.0f, 1.1f, 1.2f);
			glm::f32vec3 r = glm::f32vec3(2.0f, 2.1f, 2.2f);
			glm::f32vec3 s = glm::f32vec3(3.0f, 3.1f, 3.2f);

			OpenGLTexture tex = OpenGLTexture("buddha", "asset/bussha.png");

			auto ro = RenderObject(&tex, &p, &r, &s);

			REQUIRE(ro.get_texture() != nullptr);
			REQUIRE(ro.get_texture()->filepath == tex.filepath);

			auto pos = ro.get_position();
			auto roa = ro.get_rotation();
			auto scl = ro.get_scale();

			REQUIRE(typeid(pos) == typeid(glm::f32vec3));
			REQUIRE(typeid(roa) == typeid(glm::f32vec3));
			REQUIRE(typeid(scl) == typeid(glm::f32vec3));
			
			REQUIRE(pos.x == p.x);
			REQUIRE(pos.y == p.y);
			REQUIRE(pos.z == p.z);

			REQUIRE(roa.x == r.x);
			REQUIRE(roa.y == r.y);
			REQUIRE(roa.z == r.z);

			REQUIRE(scl.x == s.x);
			REQUIRE(scl.y == s.y);
			REQUIRE(scl.z == s.z);
		}

		TEST_CASE(" RenderObject:: get/set position()  gets/sets correctly")
		{
			glm::f32vec3 a = { 0.0f, 0.0f, 0.0f };
			glm::f32vec3 b = { 1.0f, 1.0f, 1.0f };

			auto ro = RenderObject();

			ro.set_position(b);
			auto pos = ro.get_position();

			REQUIRE_FALSE(pos == a);
			REQUIRE(pos == b);
		}

		TEST_CASE(" RenderObject:: get/set rotation()  gets/sets correctly")
		{
			glm::f32vec3 a = { 0.0f, 0.0f, 0.0f };
			glm::f32vec3 b = { 1.0f, 1.0f, 1.0f };

			auto ro = RenderObject();

			ro.set_rotation(b);
			auto roa = ro.get_rotation();

			REQUIRE_FALSE(roa == a);
			REQUIRE(roa == b);
		}

		TEST_CASE(" RenderObject:: get/set scale()   gets/sets correctly")
		{
			glm::f32vec3 a = { 0.0f, 0.0f, 0.0f };
			glm::f32vec3 b = { 1.0f, 1.0f, 1.0f };

			auto ro = RenderObject();

			ro.set_scale(b);
			auto scale = ro.get_scale();

			REQUIRE_FALSE(scale == a);
			REQUIRE(scale == b);
		}

		TEST_CASE(" RenderObject:: get/set is_visible()   gets/sets correctly")
		{
			auto ro = RenderObject();
			REQUIRE_NOTHROW(ro.set_is_visible(true));
			REQUIRE(ro.is_visible());
			ro.set_is_visible(false);
			REQUIRE(ro.is_visible() == false);
		}

		TEST_CASE(" RenderObject:: get/set texture()  gets/sets current texture")
		{
			auto ro = RenderObject();

			auto texture = ro.get_texture();

			REQUIRE(texture == nullptr);

			OpenGLTexture tex("buddha", "asset/buddha.png");
			ro.set_texture(tex);

			texture = ro.get_texture();
			REQUIRE(texture != nullptr);
		}

	#endif // RUN_UNIT_TESTS

#endif // !RENDER_OBJECT_TWO_D__TST_CC
