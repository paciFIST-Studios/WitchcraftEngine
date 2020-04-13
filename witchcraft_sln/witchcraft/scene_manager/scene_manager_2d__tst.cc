#ifndef TEST_2D_SCENE_MANAGER_CC
#define TEST_2D_SCENE_MANAGER_CC

	// this header defines RUN_UNIT_TESTS, if testing is on
	#include "../unit_testing_switch.h"

	#ifdef RUN_UNIT_TESTS	
		#include "../catch.hpp"
		
		#include "scene_manager_2d.h"
		
		TEST_CASE(" SceneManager2D:: ctor")
		{
			std::string const test_value = "test_layer";

			auto manager = &SceneManager2D();
			REQUIRE(manager != nullptr);

			auto layer = manager->add_layer(test_value);
			REQUIRE(layer->get_name() == test_value);
			REQUIRE(layer->get_is_visible() == uninit::BOOL);
			REQUIRE(layer->get_z_order() == uninit::UINT);

			auto tuple = layer->get_offset();
			REQUIRE(std::get<0>(tuple) == uninit::FLOAT);
			REQUIRE(std::get<1>(tuple) == uninit::FLOAT);
		}

		TEST_CASE(" SceneManager2D:: add_layer")
		{
			std::string const test_name = "test_layer";
			auto manager = &SceneManager2D();
			REQUIRE(manager != nullptr);
			
			auto layer = manager->add_layer(test_name);
			REQUIRE(layer->get_name() == test_name);
		}

		TEST_CASE(" SceneManager2D:: find_layer")
		{
			std::string const test_name = "test_layer";
			auto manager = &SceneManager2D();
			REQUIRE(manager != nullptr);

			auto a = manager->add_layer(test_name);
			auto b = manager->find_layer(test_name);
			REQUIRE(a->get_name() == b->get_name());
		}

		TEST_CASE(" ")
		{
			std::string const test_name = "test_layer";
			auto manager = &SceneManager2D();
			REQUIRE(manager != nullptr);

			auto layer_1 = manager->add_layer(test_name);
			REQUIRE(layer_1->get_name() == test_name);
			manager->drop_layer(test_name);
			auto layer_2 = manager->find_layer(test_name);
			REQUIRE(layer_2 == nullptr);
		}


	#endif // RUN_UNIT_TESTS

#endif // !TEST_2D_SCENE_MANAGER_CC
