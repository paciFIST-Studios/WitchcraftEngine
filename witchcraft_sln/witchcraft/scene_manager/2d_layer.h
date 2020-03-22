#ifndef TWO_D_LAYER_H
#define TWO_D_LAYER_H

#include <string>

#include "../engine/engine.h"
#include "scene_object.h"

class c2DLayer : public cEngineObject
{
private:
protected:
public:

	static bool			constexpr UNINIT_VISIBILITY		= false;
	static unsigned int constexpr UNINIT_Z_ORDER		= 0;
	static float		constexpr UNINIT_COORDINATE_F	= 0.0;

	c2DLayer()
		: _layer_is_visible(UNINIT_VISIBILITY)
		, _layer_z_order(UNINIT_Z_ORDER)
		, _position_x(UNINIT_COORDINATE_F)
		, _position_y(UNINIT_COORDINATE_F)
		, _name{std::string("un-init")}
	{
	}

	bool _layer_is_visible;
	unsigned int _layer_z_order;

	float _position_x;
	float _position_y;

	std::string _name;

	std::vector<std::unique_ptr<cSceneObject>> _scene_objects;

	void update();
};

#endif // !TWO_D_LAYER_H
