#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "../message_bus/message_bus.h"
#include "../resource_manager/engine_resource_base.h"

class AudioManager : public EngineObjectBase
{
private:
protected:

	MessageBus * message_bus = nullptr;
	void handle_message(Message m) {}

	unsigned int audio_channel_id = 0;
	unsigned int engine_channel_id = 0;
	unsigned int resource_channel_id = 0;

public:

	AudioManager() {}
	~AudioManager() {}

	AudioManager(MessageBus * mb)
		: message_bus(mb)
	{
		std::function<void(Message)> cb = std::bind(&AudioManager::handle_message, this, std::placeholders::_1);
		message_bus->subscribe("audio", cb);

		audio_channel_id = message_bus->channel_lookup("audio");
		engine_channel_id = message_bus->channel_lookup("engine");
		resource_channel_id = message_bus->channel_lookup("resource");
	}

};

#endif // !AUDIO_MANAGER_H