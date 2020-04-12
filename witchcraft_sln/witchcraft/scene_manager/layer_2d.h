#ifndef LAYER_TWO_D_H
#define LAYER_TWO_D_H

#include <string>

#include "../engine/engine_object.h"
#include "scene_object.h"

class Layer2D : public qEngineObject
{
private:
protected:
public:

	Layer2D()
		: _layer_is_visible(uninit::BOOL)
		, _layer_z_order(uninit::UINT)
		, position_x_(uninit::FLOAT)
		, position_y_(uninit::FLOAT)
		, _name{std::string(uninit::CSTRING)}
	{}

	bool _layer_is_visible;
	unsigned int _layer_z_order;

	float position_x_;
	float position_y_;

	std::string _name;

	std::vector<std::unique_ptr<cSceneObject>> _scene_objects;

	void update();
};

#endif // !LAYER_TWO_D_H
