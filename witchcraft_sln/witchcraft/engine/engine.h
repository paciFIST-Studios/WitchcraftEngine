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
public:

	cResourceManager * resource = nullptr;
	cRenderManager * render = nullptr;
	cSceneManager * scene = nullptr;
	//std::unique_ptr<cMessageManager> message;

	void startup();
	void run();
	void shutdown();

	cEngine() {}
};

#endif // !ENGINE_H
