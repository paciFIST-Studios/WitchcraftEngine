#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H


#include "../message_bus/message_bus.h"
#include "../resource_manager/engine_resource_base.h"

#include "../string_constants.h"
#include "../utility/utility.h"

class AudioManager : public EngineObjectBase
{
private:
protected:

	MessageBus * message_bus = nullptr;

	void handle_message(Message m)
	{
		message_bus->log_message(m);

		if (m.type == MessageType::REQUEST__AUDIO_STOP_PLAY)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
		}
		else if (m.type == MessageType::REQUEST__AUDIO_PLAY)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
		}
		else if (m.type == MessageType::REQUEST__AUDIO_PLAY_LOOP)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
		}
		else if (m.type == MessageType::REQUEST__AUDIO_LOAD)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
		}
		else if (m.type == MessageType::REQUEST__AUDIO_LOAD_AND_PLAY)
		{
			auto path = static_cast<std::string *>(m.data)->c_str();
		}
	}

	unsigned int audio_channel_id = 0;
	unsigned int engine_channel_id = 0;
	unsigned int resource_channel_id = 0;


public:

	AudioManager() {}
	~AudioManager() 
	{
	}

	AudioManager(MessageBus * mb)
		: message_bus(mb)
	{
		std::function<void(Message)> cb = std::bind(&AudioManager::handle_message, this, std::placeholders::_1);
		message_bus->subscribe("audio", cb);

		audio_channel_id = message_bus->channel_lookup("audio");
		engine_channel_id = message_bus->channel_lookup("engine");
		resource_channel_id = message_bus->channel_lookup("resource");

	}
}; // !AudioManager

#endif // !AUDIO_MANAGER_H