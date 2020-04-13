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

	}
}

#endif // !CALLBACK_TYPES_H
