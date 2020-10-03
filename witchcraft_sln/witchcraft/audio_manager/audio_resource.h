#ifndef AUDIO_RESOURCE_H
#define AUDIO_RESOURCE_H

#include "../resource_manager/engine_resource_base.h"

class AudioResource : public EngineResourceBase
{
private:
protected:
public:

	AudioResource() : EngineResourceBase() {}
	~AudioResource(){}

	AudioResource(
		  std::string const & name
		, std::string const & filepath
		, EResourceType type
		, unsigned scope
	)
	: EngineResourceBase(name, filepath, type, scope)
	{}

	void load() override {}
	void unload() override {}

};

#endif // !AUDIO_RESOURCE_H