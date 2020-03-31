#ifndef ENGINE_H
#define ENGINE_H

#include "../engine/engine_object.h"

#include "../resource_manager/resource_manager.h"
#include "../render_manager/2d_render_manager.h"

class qResourceManager;
class q2DRenderManager;

enum class EEngineState : unsigned char
{
	  UNINIT		= 0x01
	, CONSTRUCTED	= 0x02
	, STARTUP		= 0x04
	, RUNNING		= 0x08
	, SHUTDOWN		= 0x10
	// 0x20
	// 0x40
	// 0x80
};

struct EngineInitializer
{
	unsigned int id;
	bool tm_early_exit;
};

class cEngine : public qEngineObject
{
private:
protected:

	//std::unique_ptr<cMessageManager> message;
	std::unique_ptr<qResourceManager> resource;
	std::unique_ptr<q2DRenderManager> render;
	//std::unique_ptr<cSceneManager> scene;

	EEngineState _current_engine_state = EEngineState::UNINIT;

	// testing modes
	bool tm_early_exit;

public:
	
	void startup();
	void run();
	void shutdown();

	EEngineState get_current_state() const { return _current_engine_state; }

	cEngine() 
	: _current_engine_state(EEngineState::CONSTRUCTED) 
	{}

	cEngine(EngineInitializer init) 
	: _current_engine_state(EEngineState::CONSTRUCTED)
	{
		id = init.id;
		tm_early_exit = init.tm_early_exit;
	}
};

#endif // !ENGINE_H
