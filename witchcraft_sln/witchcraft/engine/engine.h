#ifndef ENGINE_H
#define ENGINE_H



#include "../engine/engine_object.h"


class cResourceManager;
class cRenderManager;
class cSceneManager;

class cEngine : public cEngineObject
{
private:
protected:

	//std::unique_ptr<cMessageManager> message;
	//std::unique_ptr<cResourceManager> resource;
	//std::unique_ptr<cRenderManager> render;
	//std::unique_ptr<cSceneManager> scene;

public:


	void startup();
	void run();
	void shutdown();

	cEngine() {}
};

#endif // !ENGINE_H
