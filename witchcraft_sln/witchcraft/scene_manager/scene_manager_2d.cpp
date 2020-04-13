#include "scene_manager_2d.h"

Layer2D * SceneManager2D::add_layer(std::string const & name)
{
	auto found = find_layer(name);

	if (found == nullptr)
	{
		Layer2DInitializer init;
		init.name = name;
		init.is_visible = uninit::BOOL;
		init.zOrder = uninit::UINT;
		init.x = uninit::FLOAT;
		init.y = uninit::FLOAT;
		auto layer = std::make_unique<Layer2D>(init);
	
		auto result = layer.get();
		layers.push_back(std::move(layer));
		return result;
	}

	return nullptr;
}

Layer2D * SceneManager2D::find_layer(std::string const & name)
{
	for (auto&& layer : layers)
	{
		if (layer->get_name() == name)
		{
			return layer.get();
		}
	}

	return nullptr;
}

void SceneManager2D::drop_layer(std::string const & name)
{
	for (int i = 0; i < layers.size(); i++)
	{
		if (layers[i]->get_name() == name)
		{
			// reset in-place, so we don't jostle the underlying array
			layers[i] = std::make_unique<Layer2D>();
			break;
		}
	}
}
