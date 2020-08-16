#include "scene_manager_2d.h"

Layer2D * SceneManager2D::add_layer(std::string const & name)
{
	auto found = find_layer(name);// if found {return} else {construct, return}

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
	if (layers.empty()) { return nullptr; }

	for (auto&& layer : layers)
	{
		// our array is keeping active unique_ptr objects,
		// but the layers they point to can be ejected,
		// leaving a unique_ptr that points to nullptr
		if (layer.get() == nullptr) continue;

		if (layer->get_name() == name)
		{
			return layer.get();
		}
	}

	return nullptr;
}

std::unique_ptr<Layer2D> SceneManager2D::eject_layer(std::string const & name)
{
	std::unique_ptr<Layer2D> result = nullptr;

	for (unsigned int i = 0; i < layers.size(); i++)
	{
		if (layers[i]->get_name() == name)
		{
			result = std::move(layers[i]);
			break;
		}
	}

	return std::move(result);
}



void SceneManager2D::sort_layers()
{
	std::unique_ptr<Layer2D> swap = nullptr;
	unsigned int i = 1;
	while (i < layers.size())
	{
		unsigned int j = i;
		while (j > 0 && layers[j-1]->get_z_order() > layers[j]->get_z_order())
		{
			swap = std::move(layers[j]);
			layers[j] = std::move(layers[j-1]);
			layers[j - 1] = std::move(swap);
			j -= 1;
		}
		i += 1;
	}
}
