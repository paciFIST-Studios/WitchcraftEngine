#ifndef SCENE_MANAGER_TWO_D_H
#define SCENE_MANAGER_TWO_D_H

#include <functional>
#include <string>
#include <vector>

#include "../../lib/rapidxml/rapidxml.hpp"
#include "../../lib/rapidxml/rapidxml_utils.hpp"
#define XML rapidxml

#include "../engine/engine_object.h"
#include "../utility/callback_types.h"

#include "../render_manager/render_manager_2d.h"

#include "layer_2d.h"
#include "scene_listener.h"
#include "timer.h"

class RenderManager2D;

class SceneManager2D : public qEngineObject
{
public:
	typedef std::vector<std::unique_ptr<Layer2D>> LayerVectorType;
	typedef std::vector<std::unique_ptr<TickTimer>> TickTimerType;
	typedef std::vector<std::unique_ptr<qSceneListener>> SceneListenerType;

private:
protected:
	void add_to_layer(Layer2D * layer, XML::xml_node<> const & xml);
	void check_timers();
	void sort_layers();


	LayerVectorType layers;
	//TickTimerType timers;
	//SceneListenerType listeners;

	RenderManager2D * render_manager = nullptr;

public:

	Layer2D * add_layer(std::string const & name);
	Layer2D * find_layer(std::string const & name);
	std::unique_ptr<Layer2D> eject_layer(std::string const & name);
	
	bool load_from_xml(std::string const & file);
	void add_timer(unsigned int time_len, std::function<void()> cb);
	//void add_listener(qSceneListener * listener);


	void set_render_manager(RenderManager2D * rm) { render_manager = rm; }

	void update();
};


#endif // !SCENE_MANAGER_TWO_D_H
