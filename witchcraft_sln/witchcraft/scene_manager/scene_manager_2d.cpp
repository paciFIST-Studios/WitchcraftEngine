#include "scene_manager_2d.h"

Layer2D * SceneManager2D::add_layer(std::string const & name)
{
	auto found = find_layer(name);

	//if (found == nullptr)
	//{
	//	Layer2DInitializer init;
	//	init.is_visible = false;
	//	init.name = name;
	//	init.zOrder = 0;
	//	auto layer = std::make_unique<Layer2D>(init);
	//
	//	auto result = layer.get();
	//	layers.push_back(std::move(layer));
	//	return result;
	//}

	return nullptr;
}

Layer2D * SceneManager2D::find_layer(std::string const & name)
{
	//for (auto&& layer : layers)
	//{
	//	if (layer->get_name() == name)
	//	{
	//		return layer.get();
	//	}
	//}

	return nullptr;
}

void SceneManager2D::drop_layer(std::string const & name)
{
	//for (auto&& layer : layers)
	//{
	//	if (layer->get_name() == name)
	//	{
	//		// moves unique_ptr into this scope, and then lets go
	//		std::move(layer);
	//	}
	//}
}
