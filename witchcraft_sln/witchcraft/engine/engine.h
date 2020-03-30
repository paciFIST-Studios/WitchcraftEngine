#ifndef ENGINE_H
#define ENGINE_H

#include "../engine/engine_object.h"

#include "../resource_manager/resource_manager.h"

class cResourceManager;
class cRenderManager;
class cSceneManager;


struct EngineInitializer
{
	unsigned int id;
};

class cEngine : public cEngineObject
{
private:
protected:

	//std::unique_ptr<cMessageManager> message;
	std::unique_ptr<cResourceManager> resource;
	//std::unique_ptr<cRenderManager> render;
	//std::unique_ptr<cSceneManager> scene;

public:
	
	void startup();
	void run();
	void shutdown();

	cEngine() {}
	cEngine(EngineInitializer init) 
	{
		id = init.id;
	}
};

#endif // !ENGINE_H
