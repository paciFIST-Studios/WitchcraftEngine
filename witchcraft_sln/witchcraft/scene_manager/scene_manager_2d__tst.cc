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

		TEST_CASE(" SceneManager2D:: eject_layer")
		{
			std::string const test_name = "test_layer";
			auto manager = &SceneManager2D();
			REQUIRE(manager != nullptr);

			auto layer_1 = manager->add_layer(test_name);
			REQUIRE(layer_1->get_name() == test_name);
			auto layer_2 = manager->eject_layer(test_name);
			REQUIRE(layer_1->get_name() == layer_2->get_name());
			auto layer_3 = manager->find_layer(test_name);
			REQUIRE(layer_3 == nullptr);
		}


		class TestSceneManager2D : public SceneManager2D
		{
		public:
			void invoke_sort_layers() { sort_layers(); }

			std::vector<unsigned int> get_current_layer_order()
			{
				std::vector<unsigned int> result;
				for (auto&& layer : layers)	{ result.push_back(layer->get_z_order()); }
				return result;
			}
		};

		TEST_CASE(" SceneManager2D:: sort_layers()")
		{
			std::string const prepend_name = "layer_";
			auto manager = &TestSceneManager2D();
			REQUIRE(manager != nullptr);

			// add layers in reverse order
			auto layer_3 = manager->add_layer(prepend_name + "3");
			auto layer_2 = manager->add_layer(prepend_name + "2");
			auto layer_1 = manager->add_layer(prepend_name + "1");

			// specify z order, to match name
			layer_1->set_z_order(1);
			layer_2->set_z_order(2);
			layer_3->set_z_order(3);

			// test to make sure they're out of order right now
			{
				auto order = manager->get_current_layer_order();
				REQUIRE(order[0] == 3);
				REQUIRE(order[1] == 2);
				REQUIRE(order[2] == 1);
			}

			manager->invoke_sort_layers();
	
			auto order = manager->get_current_layer_order();
			unsigned int last_z_index = 0;
			for (auto&& element : order)
			{
				REQUIRE(element >= last_z_index);
				last_z_index = element;
			}
		}

	#endif // RUN_UNIT_TESTS

#endif // !TEST_2D_SCENE_MANAGER_CC
