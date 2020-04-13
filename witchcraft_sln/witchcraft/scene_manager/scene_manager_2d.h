#ifndef SCENE_MANAGER_TWO_D_H
#define SCENE_MANAGER_TWO_D_H

#include <functional>
#include <string>
#include <vector>

#include "../../lib/rapidxml/rapidxml.hpp"
#include "../../lib/rapidxml/rapidxml_utils.hpp"
#define XML rapidxml

#include "../engine/engine_object.h"

#include "layer_2d.h"
#include "scene_listener.h"
#include "timer.h"


#define LAYER_VECTOR std::vector<std::unique_ptr<Layer2D>>
#define TIMER_VECTOR std::vector<std::unique_ptr<TickTimer>>
#define LISTEN_VECTOR std::vector<std::unique_ptr<qSceneListener>>

class SceneManager2D : public qEngineObject
{
private:
protected:
	void add_to_layer(Layer2D * layer, XML::xml_node<> const & xml);
	void check_timers();
	void sort_layers();


	LAYER_VECTOR layers;
	//TIMER_VECTOR timers;
	//LISTEN_VECTOR listeners;

public:

	Layer2D * add_layer(std::string const & name);
	Layer2D * find_layer(std::string const & name);
	std::unique_ptr<Layer2D> eject_layer(std::string const & name);
	
	bool load_from_xml(std::string const & file);
	void add_timer(unsigned int time_len, std::function<void()> cb);
	//void add_listener(qSceneListener * listener);


	void update();
};


#endif // !SCENE_MANAGER_TWO_D_H
