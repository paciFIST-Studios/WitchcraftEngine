#ifndef LAYER_TWO_D_H
#define LAYER_TWO_D_H

#include <string>

#include "../engine/engine_object.h"
#include "scene_object.h"


struct Layer2DInitializer
{
	unsigned int ID;
	unsigned int zOrder;
	float x;
	float y;
	bool is_visible;
	std::string name;
};

class Layer2D : public qEngineObject
{
private:
protected:
	std::string const name;

	unsigned int z_order;

	bool visible;

	float pos_x;
	float pos_y;


	std::vector<std::unique_ptr<qSceneObject>> scene_objects;


public:

	Layer2D()
		: qEngineObject()
		, name{ std::string(uninit::CSTRING) }
		, z_order(uninit::UINT)
		, visible(uninit::BOOL)
		, pos_x(uninit::FLOAT)
		, pos_y(uninit::FLOAT)
	{}

	Layer2D(Layer2DInitializer init)
		: qEngineObject(init.ID)
		, z_order(init.zOrder)
		, visible(init.is_visible)
		, pos_x(init.x)
		, pos_y(init.y)
		, name(init.name)
	{}
	
	~Layer2D() {}
	

	
	void update();



	// vis
	bool get_is_visible() const { return visible; }
	void set_is_visible(bool is) { visible = is; }

	// z order
	unsigned int get_z_order() const { return z_order; }
	void set_z_order(unsigned int z) { z_order = z; }

	// xy pos of layer itself
	std::tuple<float, float> get_offset() const { return std::make_tuple(pos_x, pos_y); }
	void set_offset(float x, float y) { pos_x = x; pos_y = y; }

	std::string get_name() const { return name; }

};

#endif // !LAYER_TWO_D_H
