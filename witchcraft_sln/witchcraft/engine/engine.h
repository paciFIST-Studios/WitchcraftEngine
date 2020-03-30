#ifndef ENGINE_H
#define ENGINE_H

#include "../engine/engine_object.h"

class cEngine : public cEngineObject
{
private:
protected:
public:

	//std::unique_ptr<cResourceManager> resource;
	//std::unique_ptr<cRenderManager> render;
	//std::unique_ptr<cSceneManager> scene;
	//std::unique_ptr<cMessageManager> message;

	void startup();
	void run();
	void shutdown();

	cEngine() {}
};

#endif // !ENGINE_H
