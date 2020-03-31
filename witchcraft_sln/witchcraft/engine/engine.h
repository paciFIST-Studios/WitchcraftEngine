#ifndef ENGINE_H
#define ENGINE_H

#include "../engine/engine_object.h"

#include "../resource_manager/resource_manager.h"

class cResourceManager;

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
};

class cEngine : public cEngineObject
{
private:
protected:

	//std::unique_ptr<cMessageManager> message;
	std::unique_ptr<cResourceManager> resource;
	//std::unique_ptr<cRenderManager> render;
	//std::unique_ptr<cSceneManager> scene;

	EEngineState _current_engine_state = EEngineState::UNINIT;

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
	}
};

#endif // !ENGINE_H
