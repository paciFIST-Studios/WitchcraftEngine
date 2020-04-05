#ifndef TWO_D_LAYER_H
#define TWO_D_LAYER_H

#include <string>

#include "../engine/engine_object.h"
#include "scene_object.h"

class c2DLayer : public qEngineObject
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
		, position_x_(UNINIT_COORDINATE_F)
		, position_y_(UNINIT_COORDINATE_F)
		, _name{std::string("un-init")}
	{}

	bool _layer_is_visible;
	unsigned int _layer_z_order;

	float position_x_;
	float position_y_;

	std::string _name;

	std::vector<std::unique_ptr<cSceneObject>> _scene_objects;

	void update();
};

#endif // !TWO_D_LAYER_H
