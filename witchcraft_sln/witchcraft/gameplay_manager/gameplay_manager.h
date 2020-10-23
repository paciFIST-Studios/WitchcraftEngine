#ifndef GAMEPLAY_MANAGER_H
#define GAMEPLAY_MANAGER_H

#include "../engine/engine_object.h"
#include "../message_bus/message_bus.h"

class GameplayManager : public EngineObjectBase
{
private:
protected:
	MessageBus * message_bus = nullptr;

public:
	GameplayManager() {}
	GameplayManager(MessageBus * mb) : message_bus(mb) {}
	~GameplayManager() { message_bus = nullptr; }

	void init_component()
	{
		// load gameplay
	}
};


#endif // !GAMEPLAY_MANAGER_H