#ifndef CALLBACK_TYPES_H
#define CALLBACK_TYPES_H

#include <functional>
#include <memory>
#include <vector>

class Layer2D;

namespace witchcraft
{
	namespace callback_types
	{
		typedef std::function<std::vector<std::unique_ptr<Layer2D>>const&()> SceneLayersCallbackType;
		typedef std::function<std::vector< Layer2D*>()> SceneLayersCallbackType2;
		typedef std::vector<Layer2D *> SceneLayersPtrsVector;
	}
}

#endif // !CALLBACK_TYPES_H
