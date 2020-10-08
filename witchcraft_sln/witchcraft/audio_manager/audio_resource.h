#ifndef AUDIO_RESOURCE_H
#define AUDIO_RESOURCE_H

#include "../resource_manager/engine_resource_base.h"

#include "../message_bus/message_bus.h"

class AudioResource : public EngineResourceBase
{
private:
protected:
	MessageBus * message_bus;
	unsigned int audio_channel_id;
public:

	AudioResource() : EngineResourceBase() {}
	~AudioResource(){}

	AudioResource(
		  std::string const & name
		, std::string const & filepath
		, EResourceType type
		, unsigned scope
		, MessageBus * mb
	)
	: EngineResourceBase(name, filepath, type, scope)
	, message_bus(mb)
	{}

	void load() override 
	{
		Message request{
			audio_channel_id
			, this->id
			, MessageType::REQUEST__AUDIO_LOAD
			, (void*) filepath.c_str()
		};
		message_bus->send_direct_message(request);
	}


	void unload() override 
	{
		//Message request{
		//	audio_channel_id
		//	, this->id
		//	, MessageType::REQUEST__AUDIO_UNLOAD
		//	, (void*) filepath.c_str()
		//};
		//message_bus->send_direct_message(request);
	}

};

#endif // !AUDIO_RESOURCE_H